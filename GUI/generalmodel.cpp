#include "generalmodel.h"

GeneralModel::GeneralModel(QObject* parent, PhysicalQuantity* physicalquantity) :
              QAbstractItemModel(parent),
              physicalquantity(physicalquantity),
              rootNode(new TreeNode("Root")),
              h5filemanager(H5FileManager::instance()) {
    initName = {
        {Grid::Initialization::EQUILIBRIUM, "Равновестная"},
        {Grid::Initialization::NONEQUILIBRIUM, "Неравновестная"}
    };

    unitSystemName = {
        {Unit::System::SI, "СИ"},
        {Unit::System::TS, "Промысловые"}
    };

    dateNode = new TreeNode("Дата начала расчета", rootNode);
    rootNode->children.append(dateNode);

    unitNode = new TreeNode("Система едениц измерения", rootNode);
    rootNode->children.append(unitNode);

    phaseNode = new TreeNode("Фазы", rootNode);
    phaseNode->children.append(new TreeNode("Нефть", phaseNode));
    phaseNode->children.append(new TreeNode("Вода", phaseNode));
    phaseNode->children.append(new TreeNode("Газ", phaseNode));
    phaseNode->children.append(new TreeNode("Растворенный газ", phaseNode));
    phaseNode->children.append(new TreeNode("Испаренная нефть", phaseNode));
    rootNode->children.append(phaseNode);

    initNode = new TreeNode("Тип инициализации", rootNode);
    rootNode->children.append(initNode);

    regionNode = new TreeNode("Количество регионов", rootNode);
    regionNode->children.append(new TreeNode("Отчета", regionNode));
    regionNode->children.append(new TreeNode("PVT", regionNode));
    regionNode->children.append(new TreeNode("Фильтраций", regionNode));
    regionNode->children.append(new TreeNode("Свойств породы", regionNode));
    regionNode->children.append(new TreeNode("Равновесия", regionNode));
    rootNode->children.append(regionNode);
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
            Unit::System val = unitSystemName.key(value.toString());
            h5filemanager.setUnitSystem(val);
            physicalquantity->setUnitSystem(val);
        } else if (node == initNode) {
            node->value = static_cast<int>(initName.key(value.toString())); // QVariant::fromValue(
        } else if (node->parent == regionNode) {
            node->parent->value= value.toInt();
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
                return unitSystemName[h5filemanager.unitSystem()];
            }
            if (node == initNode) {
                return initName[h5filemanager.typeInitialization()];
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
        if (index.column() == 0 && node->parent == rootNode) {
            QFont font;
            font.setBold(true);
            return font;
        }
        break;
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

    Grid::Initialization init = static_cast<Grid::Initialization>(initNode->value.toInt());

    h5filemanager.setStartDate(arrdate);
    h5filemanager.setFluids(fluids);
    h5filemanager.setRegions(regions);
    h5filemanager.setTypeInitialization(init);
}

void GeneralModel::loadData() {

}


int GeneralModel::TreeNode::row() const {
    if (parent) {
        return parent->children.indexOf(const_cast<TreeNode*>(this));
    }
    return 0;
}
