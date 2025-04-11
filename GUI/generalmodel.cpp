#include "generalmodel.h"

GeneralModel::GeneralModel(QObject* parent, PhysicalQuantity* physicalquantity) :
              QAbstractItemModel(parent),
              physicalquantity(physicalquantity),
              rootNode(new TreeNode("Общее")),
              h5filemanager(H5FileManager::instance()) {
    bold.setBold(true);

    init = {
        {Grid::Initialization::EQUILIBRIUM, "Равновестная"},
        {Grid::Initialization::NONEQUILIBRIUM, "Неравновестная"}
    };

    unit = {
        {Unit::System::SI, "СИ"},
        {Unit::System::TS, "Промысловые"}
    };

    dateNode = new TreeNode("Дата начала расчета", QDateTime::currentDateTime(), rootNode);
    unitNode = new TreeNode("Система едениц измерения",unit[Unit::System::TS], rootNode);
    phaseNode = new TreeNode("Фазы",QVariant(), rootNode);
    initNode = new TreeNode("Тип инициализации", init[Grid::Initialization::EQUILIBRIUM], rootNode);
    regionNode = new TreeNode("Количество регионов" ,QVariant(), rootNode);

    rootNode->children.append({
        dateNode,
        unitNode,
        phaseNode,
        initNode,
        regionNode
    });

    phaseNode->children.append({
        new TreeNode("Вода", true, phaseNode),
        new TreeNode("Нефть", false, phaseNode),
        new TreeNode("Газ", false, phaseNode),
        new TreeNode("Растворенный газ",false, phaseNode),
        new TreeNode("Испаренная нефть",false, phaseNode)
    });

    regionNode->children.append({
        new TreeNode("Отчета",1, regionNode),
        new TreeNode("PVT",1, regionNode),
        new TreeNode("Фильтраций",1, regionNode),
        new TreeNode("Свойств породы",1, regionNode),
        new TreeNode("Равновесия",1, regionNode)
    });
}


GeneralModel::~GeneralModel() {
    delete rootNode;
}


bool GeneralModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid() || index.column() != 1) {
        return false;
    }

    TreeNode* node = static_cast<TreeNode*>(index.internalPointer());

    switch (role) {
    case Qt::EditRole:
        if (node == dateNode) {
            startDate = value.toDateTime();
        } else if (node == unitNode) {
            node->value = value.toString();
        } else if (node == initNode) {
            node->value = value.toString();
        } else if (node->parent == regionNode) {
            node->value= value.toInt();
        } else {
            return false;
        }
        emit dataChanged(index, index);
        return true;

    case Qt::CheckStateRole:
        if (node->parent == phaseNode) {
            node->value = value.toBool();
        }
        emit dataChanged(index, index);
    default:
        return false;
    }
}


QVariant GeneralModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    TreeNode* node = static_cast<TreeNode*>(index.internalPointer());

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (index.column() == 0) {
            return node->name;
        }
        if (index.column() == 1) {
            if (node == dateNode) {
                return node->value.toDateTime();
            }
            if (node == unitNode) {
                return node->value.toString();
            }
            if (node == initNode) {
                return node->value.toString();
            }
            if (node->parent == regionNode) {
                return node->value.toInt();
            }
            if (node->parent == phaseNode && role == Qt::EditRole) {
                return node->value.toBool();
            }
        }
        break;

    case Qt::CheckStateRole:
        if (index.column() == 1 && node->parent == phaseNode) {
            return index.data(Qt::EditRole).toBool() ? Qt::Checked : Qt::Unchecked;
        }
        break;

    case Qt::UserRole + 1:
        if (node == dateNode) {
            return QVariant::fromValue(DelegateType::DateEditDelegate);
        }
        if (node == unitNode || node == initNode) {
            return QVariant::fromValue(DelegateType::ComboBoxDelegate);
        }
        if (node->parent == regionNode) {
            return QVariant::fromValue(DelegateType::IntSpinBoxDelegate);
        }
        break;

    case Qt::UserRole + 2:
        if (node == unitNode) {
            return QStringList{"Промысловые", "СИ"};
        } else if (node == initNode) {
            return QStringList{"Равновестная", "Неравновестная"};
        }
        break;

    case Qt::FontRole:
        if (index.column() == 0 && node->parent == rootNode) return QFont(bold);
    }

    return QVariant();
}


Qt::ItemFlags GeneralModel::flags(const QModelIndex& index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.column() == 1) {
        TreeNode* node = static_cast<TreeNode*>(index.internalPointer());
        if (node->parent == regionNode || node == dateNode || node == unitNode || node == initNode) {
            flags |= Qt::ItemIsEditable;
        }
        if (node->parent == phaseNode) {
            flags |= Qt::ItemIsUserCheckable;
        }
    }

    return flags;
}


QVariant GeneralModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0: return "Название";
            case 1: return "Значение";
            }
        }
    }
    return QVariant();
}


QModelIndex GeneralModel::index(int row, int column, const QModelIndex& parent) const {
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    TreeNode* parentNode = parent.isValid() ? static_cast<TreeNode*>(parent.internalPointer()) : rootNode;
    TreeNode* childNode = parentNode->children.value(row);

    if (childNode) {
        return createIndex(row, column, childNode);
    }

    return QModelIndex();
}


QModelIndex GeneralModel::parent(const QModelIndex& index) const {
    if (!index.isValid()) {
        return QModelIndex();
    }

    TreeNode* childNode = static_cast<TreeNode*>(index.internalPointer());
    TreeNode* parentNode = childNode->parent;

    if (parentNode == rootNode) {
        return QModelIndex();
    }

    return createIndex(parentNode->row(), 0, parentNode);
}


int GeneralModel::rowCount(const QModelIndex& parent) const {
    TreeNode* parentNode = parent.isValid() ? static_cast<TreeNode*>(parent.internalPointer()) : rootNode;
    return parentNode->children.count();
}


int GeneralModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return 2;
}


void GeneralModel::saveData() {
    QDate date = dateNode->value.toDate();
    QTime time = dateNode->value.toTime();
    std::array<int, 6> arrdate;
    arrdate[0] = date.year();
    arrdate[1] = date.month();
    arrdate[2] = date.day();
    arrdate[3] = time.hour();
    arrdate[4] = time.minute();
    arrdate[5] = time.second();

    std::array<int,5> fluids;
    size_t item = 0;
    for (const auto& child : phaseNode->children) {
        fluids[item] = child->value.toInt();
        ++item;
    }

    std::array<int,5> regions;
    item = 0;
    for (const auto& child : regionNode->children) {
        regions[item] = child->value.toInt();
        ++item;
    }

    Grid::Initialization initval = static_cast<Grid::Initialization>(init.key(initNode->value.toString()));

    h5filemanager.setStartDate(arrdate);
    h5filemanager.setFluids(fluids);
    h5filemanager.setRegions(regions);
    h5filemanager.setTypeInitialization(initval);

    Unit::System val = unit.key( unitNode->value.toString());
    h5filemanager.setUnitSystem(val);
    physicalquantity->setUnitSystem(val);
}

void GeneralModel::loadData() {

}
