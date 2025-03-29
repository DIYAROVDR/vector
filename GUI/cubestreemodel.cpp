#include "cubestreemodel.h"


CubesTreeModel::CubesTreeModel(QObject* parent, PhysicalQuantity* physicalquantity):
               QAbstractItemModel(parent),
               physicalquantity(physicalquantity),
               rootNode(new TreeNode("Root",nullptr, PhysicalQuantity::Quantity::CUBEDATA)) {
    bold.setBold(true);

    auto init = PhysicalQuantity::Quantity::INITCUBES;
    auto reg = PhysicalQuantity::Quantity::REGIONS;
    auto user = PhysicalQuantity::Quantity::USERCUBE;

    initialCubesNode = new TreeNode("Начальные кубы", rootNode,init);
    regionsNode = new TreeNode("Регионы", rootNode, reg);
    userCubesNode = new TreeNode("Пользовательские кубы", rootNode, user);

    for(const auto& pair : physicalquantity->getCubesNameStr()) {
        QString name = QString::fromStdString(pair.second);
        if (pair.first > init && pair.first < reg) {
            initialCubesNode->children.append(new TreeNode(name,initialCubesNode,pair.first));
        }
        else if (pair.first > reg && pair.first < user) {
            regionsNode->children.append(new TreeNode(name,regionsNode,pair.first));
        }
        else if(pair.first > user) {
            userCubesNode->children.append(new TreeNode(name,userCubesNode,pair.first, true));
        }
    }

    rootNode->children.append(initialCubesNode);
    rootNode->children.append(regionsNode);
    rootNode->children.append(userCubesNode);
}


CubesTreeModel::~CubesTreeModel() {
    delete rootNode;
}



bool CubesTreeModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid()) {
        return false;
    }

    TreeNode* node = static_cast<TreeNode*>(index.internalPointer());

    if(role == Qt::EditRole) {
        if (node->isEditable) {
            node->name = value.toString();
            emit dataChanged(index, index);
            return true;
        }
    }

    else if(role == Qt::ForegroundRole) {
        if (node->parent != rootNode) {
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
    }


    return false;
}

QVariant CubesTreeModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    TreeNode* node = static_cast<TreeNode*>(index.internalPointer());

    switch (role) {
    case Qt::DisplayRole:

    case Qt::EditRole:
        return node->name;

    case Qt::DecorationRole:
        if (node->parent == rootNode) {
            return QVariant();
        }
        else {
            return node->isEmpty ? QIcon(":/images/empty_cube.png") : QIcon(":/images/fill_cube.png");
        }

    case Qt::ForegroundRole:
        return node->select ? QColor(Qt::red) : QColor(Qt::black);

    case Qt::FontRole:
        return (node->parent == rootNode || node->select) ?  QFont(bold) : QVariant();

    case Qt::UserRole + 1:
        return QString::fromStdString(physicalquantity->getCubeNameVar(node->value));

    case Qt::UserRole + 2:
        return node->select ? true : false;

    case Qt::UserRole + 3:
        return (node->parent != rootNode) ? node->name : QVariant();



    }
    return QVariant();
}

Qt::ItemFlags CubesTreeModel::flags(const QModelIndex& index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }


    TreeNode* node = static_cast<TreeNode*>(index.internalPointer());
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (node->isEditable) {
        flags |= Qt::ItemIsEditable;
    }


    return flags;
}


QVariant CubesTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return "Свойства";

    return QVariant();
}


QModelIndex CubesTreeModel::index(int row, int column, const QModelIndex& parent) const {
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    TreeNode* parentNode = parent.isValid() ? static_cast<TreeNode*>(parent.internalPointer()) : rootNode;

    if (row < parentNode->children.size()) {
        return createIndex(row, column, parentNode->children[row]);
    }

    return QModelIndex();
}

QModelIndex CubesTreeModel::parent(const QModelIndex& index) const {
    if (!index.isValid())
        return QModelIndex();

    TreeNode* node = static_cast<TreeNode*>(index.internalPointer());
    TreeNode* parentNode = node->parent;

    if (parentNode == rootNode)
        return QModelIndex();

    return createIndex(parentNode->row(), 0, parentNode);
}


int CubesTreeModel::rowCount(const QModelIndex& parent) const {
    TreeNode* parentNode = parent.isValid() ? static_cast<TreeNode*>(parent.internalPointer()) : rootNode;
    return parentNode->children.size();
}


int CubesTreeModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return 1;
}


bool CubesTreeModel::insertRow(int row, const QModelIndex& parent) {
    TreeNode* parentNode = parent.isValid() ? static_cast<TreeNode*>(parent.internalPointer()) : rootNode;

    // Проверяем, что row находится в допустимых пределах
    if (row < 0 || row > parentNode->children.size()) {
        return false;
    }


    // Начинаем вставку строки
    beginInsertRows(parent, row, row);

    // Создаем новый узел и добавляем его в children
    auto cubetype = static_cast<PhysicalQuantity::Quantity>(static_cast<int>(PhysicalQuantity::Quantity::CUBE) + row);
    TreeNode* newNode = new TreeNode("", parentNode,cubetype,true); // isEditable = true
    parentNode->children.insert(row, newNode);

    // Завершаем вставку
    endInsertRows();

    return true;
}


bool CubesTreeModel::removeRow(int row, const QModelIndex& parent) {
    TreeNode* parentNode = parent.isValid() ? static_cast<TreeNode*>(parent.internalPointer()) : rootNode;

    // Проверяем, что row находится в допустимых пределах
    if (row < 0 || row >= parentNode->children.size())
        return false;

    // Начинаем удаление строки
    beginRemoveRows(parent, row, row);

    // Удаляем узел
    TreeNode* nodeToRemove = parentNode->children.takeAt(row);
    delete nodeToRemove;

    // Завершаем удаление
    endRemoveRows();

    return true;
}


bool CubesTreeModel::hasChildren(const QModelIndex& parent) const {
    if (!parent.isValid()) {
        // Корневой элемент — проверяем, есть ли у него дети
        return !rootNode->children.isEmpty();
    }

    // Для остальных элементов проверяем, есть ли дети
    TreeNode* node = static_cast<TreeNode*>(parent.internalPointer());
    return !node->children.isEmpty();
}

int CubesTreeModel::itemLevel(const QModelIndex& index) const
{
    int level = 0;
    QModelIndex parent = index.parent();
    while (parent.isValid()) {
        level++;
        parent = parent.parent();
    }
    return level;
}


