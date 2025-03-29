#include "pvttreemodel.h"

PVTTreeModel::PVTTreeModel(PhysicalQuantity* physicalquantity, QObject* parent):
        physicalquantity(physicalquantity),
        QAbstractItemModel(parent),
        rootNode(new TreeNode("PVT")) {

    bold.setBold(true);

    regionsNode = new TreeNode("Регионы", rootNode);
    firstRegionNode = new TreeNode("DV", regionsNode, 1, Unit::Types::UNDEFINED, true);

    regionsNode->children.append(firstRegionNode);

    waterPropNode = new TreeNode("Свойства воды", rootNode);

    std::vector<double> default_data = {200E+5, 1.0, 1E-9, 1E-3, 0.0, 1000};

    int count = 0;
    for(const auto& pair : physicalquantity->getPVTDataNames()) {
        waterPropNode->children.append(new TreeNode(
                QString::fromStdString(pair.second),
                waterPropNode,
                default_data[count]
        ));
        ++count;
    }

    rootNode->children.append({
          regionsNode,
          waterPropNode
    });

    lastSelectNode = firstRegionNode;
    waterpvt.resize(1);
    waterpvt[0] = new PVTW();
}


PVTTreeModel::~PVTTreeModel() {}


QModelIndex PVTTreeModel::index(int row, int column, const QModelIndex& parent) const {
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


QModelIndex PVTTreeModel::parent(const QModelIndex& index) const {
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


int PVTTreeModel::rowCount(const QModelIndex& parent) const {
    TreeNode* parentNode = parent.isValid() ? static_cast<TreeNode*>(parent.internalPointer()) : rootNode;
    return parentNode->children.count();
}


int PVTTreeModel::columnCount(const QModelIndex& parent) const {
    return 2;
}


QVariant PVTTreeModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    TreeNode* node = static_cast<TreeNode*>(index.internalPointer());

    switch (role) {
        case Qt::DisplayRole:
            if (index.column() == 0) {
                return node->name;
            }
            else if (node->parent != rootNode) {
                return node->value;
            }
        case Qt::EditRole:
            if (node->parent != rootNode) {
                return node->value;
            }

        case Qt::UserRole + 1:
            if(node->parent == waterPropNode) {
                return QVariant::fromValue(DelegateType::ScientificNotationDelegate);
            }

        case Qt::UserRole + 3:
            if (node->parent == waterPropNode) {
                std::vector<double> data(6);
                for (size_t i = 0; i < 6; ++i) data[i] = waterPropNode->children[i]->value;
                waterpvt[lastSelectNode->row()]->setData(data);

                std::vector<double> muw = waterpvt[lastSelectNode->row()]->mutab();
                std::vector<double> bw = waterpvt[lastSelectNode->row()]->btab();
                std::vector<double> pw = waterpvt[lastSelectNode->row()]->ptab();

                // Упаковываем вектора в QVariantList
                QVariantList result;
                result.reserve(3);

                QVariantList muwList;
                for (const auto& val : muw) muwList << val;
                result << QVariant(muwList);

                QVariantList bwList;
                for (const auto& val : bw) bwList << val;
                result << QVariant(bwList);

                QVariantList pwList;
                for (const auto& val : pw) pwList << val;
                result << QVariant(pwList);

                return result;
            }

        case Qt::ForegroundRole:
            if (index.column() == 0) {
                return node->select ? QColor(Qt::red) : QColor(Qt::black);
            }

        case Qt::FontRole:
            if (index.column() == 0) {
                return (node->parent == rootNode || node->select) ?  QFont(bold) : QVariant();
            }
            break;
    };

    return QVariant();
}


QVariant PVTTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
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

Qt::ItemFlags PVTTreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.column() == 1) {
        TreeNode* node = static_cast<TreeNode*>(index.internalPointer());
        if (node->parent == waterPropNode) {
            flags |= Qt::ItemIsEditable;
        }
    }

    return flags;
}

bool PVTTreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) {
        return false;
    }

    TreeNode* node = static_cast<TreeNode*>(index.internalPointer());



    switch(role) {
        case Qt::EditRole :
            if (node->parent == waterPropNode) {
                node->value = value.toDouble();
            }
        case Qt::ForegroundRole :
            if (index.column() == 1) return false;
            if (node->parent == regionsNode) {
                if (lastSelectNode && lastSelectNode != node) {
                    lastSelectNode->select = false;
                    QModelIndex lastIndex = createIndex(lastSelectNode->row(), 0, lastSelectNode);
                    emit dataChanged(lastIndex, lastIndex, {Qt::ForegroundRole});
                }

                node->select = value.toBool();
                lastSelectNode = node;

                emit dataChanged(index, index, {Qt::ForegroundRole});
                return true;
            }
        default:
            return false;
    };
}

