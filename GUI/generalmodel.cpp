#include "generalmodel.h"

GeneralModel::GeneralModel(QObject* parent, PhysicalQuantity* physicalquantity) :
              QAbstractItemModel(parent),
              physicalquantity(physicalquantity),
              rootNode(new TreeNode("Root")),
              h5filemanager(H5FileManager::instance()) {
    fluids.resize(5);
    fluids = {true,false,false,false,false};

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
    case Qt::CheckStateRole:
        if (node == dateNode) {
            startDate = value.toString();
            h5filemanager.setStartDate(startDate.toStdString());
        } else if (node == unitNode) {
            Unit::System val = unitSystemName.key(value.toString());
            h5filemanager.setUnitSystem(val);
            physicalquantity->setUnitSystem(val);
        } else if (node == initNode) {
            h5filemanager.setTypeInitialization(initName.key(value.toString()));
        } else if (node->parent == phaseNode) {
            fluids[node->row()] = (role == Qt::EditRole) ? value.toBool() : (value.toInt() == Qt::Checked);
        } else if (node->parent == regionNode) {
            std::vector<int> regions = h5filemanager.regions();
            regions[node->row()] = value.toInt();
            h5filemanager.setRegions(regions);
        } else {
            return false;
        }
        emit dataChanged(index, index, {role, (role == Qt::CheckStateRole) ? Qt::EditRole : Qt::CheckStateRole});
        return true;

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
        } else if (index.column() == 1) {
            if (node == dateNode) {
                return QDateTime::fromString(QString::fromStdString(h5filemanager.startDate()), Qt::ISODate);
            } else if (node == unitNode) {
                return unitSystemName[h5filemanager.unitSystem()];
            } else if (node == initNode) {
                return initName[h5filemanager.typeInitialization()];
            } else if (node->parent == regionNode) {
                std::vector<int> regions = h5filemanager.regions();
                return regions[node->row()];
            } else if (node->parent == phaseNode && role == Qt::EditRole) {
                return fluids[node->row()];
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
        } else if (node == unitNode || node == initNode) {
            return QVariant::fromValue(DelegateType::ComboBoxDelegate);
        } else if (node->parent == regionNode) {
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


int GeneralModel::TreeNode::row() const {
    if (parent) {
        return parent->children.indexOf(const_cast<TreeNode*>(this));
    }
    return 0;
}
