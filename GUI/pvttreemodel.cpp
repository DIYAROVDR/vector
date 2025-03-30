#include "pvttreemodel.h"

PVTTreeModel::PVTTreeModel(PhysicalQuantity* physicalquantity, QObject* parent):
        physicalquantity(physicalquantity),
        QAbstractItemModel(parent),
        rootNode(new TreeNode("PVT")) {

    bold.setBold(true);

    regionsNode = new TreeNode("Регионы", rootNode);
    firstRegNode = new TreeNode("DV", regionsNode, 1, Unit::Types::UNDEFINED, true);

    regionsNode->children.append(firstRegNode);

    waterNode = new TreeNode("Свойства воды", rootNode);

    std::vector<double> data = {200E+5, 1.0, 1E-9, 1E-3, 0.0, 1000};

    int count = 0;
    for(const auto& pair : physicalquantity->getPVTDataNames()) {
        waterNode->children.append(new TreeNode(
                QString::fromStdString(pair.second),
                waterNode,
                physicalquantity->diconvert(data[count], pair.first),
                pair.first,
                false
        ));
        ++count;
    }

    rootNode->children.append({
          regionsNode,
          waterNode
    });

    lastSelectNode = firstRegNode;
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
            if (node->parent == regionsNode) {
                return node->value;
            }
            else if (node->parent == waterNode) {
                return node->value;
            }

        case Qt::UserRole + 1:
            if(node->parent == waterNode) {
                return QVariant::fromValue(DelegateType::ScientificNotationDelegate);
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
        if (node->parent == waterNode) {
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
            if (node->parent == waterNode) {
                node->value = value.toDouble();
                emit pvtDataChanged(); // Оповещаем об изменении
                emit dataChanged(index, index, {role});
                return true;
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
                emit pvtDataChanged(); // Оповещаем об изменении
                emit dataChanged(index, index, {Qt::ForegroundRole});
                return true;
            }
        case Qt::UserRole + 5:
            if (node == waterNode) {
                emit currentPhaseChanged();
            }
        default:
            return false;
    };
}


QStringList PVTTreeModel::tableHeaders() {
    QStringList headers = {
            "Давление",
            "Вязкость",
            "Коэффициент объемного расширения"
    };

    return headers;
}


QVector<QVector<double>> PVTTreeModel::tableData() {
    std::vector<double> data(6);
    //waterNode->children[i]->value;
    //
    for (size_t i = 0; i < 6; ++i) data[i] = physicalquantity->convert(waterNode->children[i]->value,waterNode->children[i]->type);
    waterpvt[lastSelectNode->row()]->setData(data);

    std::vector<double> muw = waterpvt[lastSelectNode->row()]->mutab();
    std::vector<double> bw = waterpvt[lastSelectNode->row()]->btab();
    std::vector<double> pw = waterpvt[lastSelectNode->row()]->ptab();

    QVector<QVector<double>> result = {
        QVector(pw.begin(), pw.end()),
        QVector(muw.begin(), muw.end()),
        QVector(bw.begin(), bw.end())
    };

    return result;
}

