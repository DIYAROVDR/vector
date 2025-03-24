#include "griddatamodel.h"

GridDataModel::GridDataModel(QObject* parent) : QAbstractItemModel(parent),
    rootNode(new TreeNode("Root")),
    h5filemanager(H5FileManager::instance()) {

    gridTypeNames = {
        {Grid::Type::BLOCKCENTERED,"Блочно-центрированная геометрия"},
        {Grid::Type::CORNERPOINT,"Геометрия угловой точки"},
        {Grid::Type::VERTEX,"Задание сетки вершинами блоков"}
    };

    gridDimensionNode = new TreeNode("Размерность сетки", rootNode);
    nxNode = new TreeNode("Nx", gridDimensionNode);
    nyNode = new TreeNode("Ny", gridDimensionNode);
    nzNode = new TreeNode("Nz", gridDimensionNode);

    gridDimensionNode->children.append(nxNode);
    gridDimensionNode->children.append(nyNode);
    gridDimensionNode->children.append(nzNode);

    rootNode->children.append(gridDimensionNode);

    numberOfLayersNode = new TreeNode("Число пластов", rootNode);
    rootNode->children.append(numberOfLayersNode);

    gridTypeNode = new TreeNode("Тип сетки", rootNode);
    rootNode->children.append(gridTypeNode);

    verticalPinchNode = new TreeNode("Вертикальное выклинивание", rootNode);
    rootNode->children.append(verticalPinchNode);

    horizontalPinchNode = new TreeNode("Горизонтальное выклинивание", rootNode);
    rootNode->children.append(horizontalPinchNode);
}

GridDataModel::~GridDataModel() {
    delete rootNode;
}

bool GridDataModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid() || index.column() != 1) {
        return false;
    }

    TreeNode* node = static_cast<TreeNode*>(index.internalPointer());

    switch (role) {
    case Qt::EditRole:
        if (node->parent == gridDimensionNode) {
            // Сохраняем размерность сетки
            std::array<int,3> dim = h5filemanager.dimens();

            int nx = dim[0];
            int ny = dim[1];
            int nz = dim[2];

            if (node == nxNode){
                nx = value.toInt();
            }
            if (node == nyNode){
                ny = value.toInt();
            }
            if (node == nzNode){
                nz = value.toInt();
            }
            h5filemanager.setDimens(nx,ny,nz);
            emit dataChanged(index, index, {role});
            // Логика сохранения в h5filemanager
        } else if (node == numberOfLayersNode) {
            // Сохраняем число пластов
            int layers = value.toInt();
            // Логика сохранения в h5filemanager
        } else if (node == gridTypeNode) {
            // Сохраняем тип сетки
            h5filemanager.setTypeGrid(gridTypeNames.key(value.toString()));
            // Логика сохранения в h5filemanager
        } else if (node == verticalPinchNode) {
            // Сохраняем вертикальное выклинивание
            QString pinchValue = value.toString();
            // Логика сохранения в h5filemanager
        } else if (node == horizontalPinchNode) {
            // Сохраняем горизонтальное выклинивание
            QString pinchValue = value.toString();
            // Логика сохранения в h5filemanager
        } else {
            return false;
        }

        return true;

    default:
        return false;
    }
}

QVariant GridDataModel::data(const QModelIndex& index, int role) const {
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
            if (node->parent == gridDimensionNode) {
                // Возвращаем значение размерности сетки
                // Сохраняем размерность сетки
                std::array<int,3> dim = h5filemanager.dimens();

                if (node == nxNode){
                    return dim[0];
                }
                if (node == nyNode){
                    return dim[1];
                }
                if (node == nzNode){
                    return dim[2];
                }

            } else if (node == numberOfLayersNode) {
                // Возвращаем число пластов
                return QVariant(); // Заглушка
            } else if (node == gridTypeNode) {
                // Возвращаем тип сетки
                return gridTypeNames.value(h5filemanager.typeGrid()); // Заглушка
            } else if (node == verticalPinchNode) {
                // Возвращаем вертикальное выклинивание
                return QVariant(); // Заглушка
            } else if (node == horizontalPinchNode) {
                // Возвращаем горизонтальное выклинивание
                return QVariant(); // Заглушка
            }
        }
        break;

    case Qt::UserRole + 1:
        if (node == gridTypeNode) {
            return QVariant::fromValue(DelegateType::ComboBoxDelegate);
        }
        break;

    case Qt::UserRole + 2:
        if (node == gridTypeNode) {
            return QStringList {
                   "Блочно-центрированная геометрия",
                   "Геометрия угловой точки",
                   "Задание сетки вершинами блоков"
            };
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

Qt::ItemFlags GridDataModel::flags(const QModelIndex& index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.column() == 1) {
        TreeNode* node = static_cast<TreeNode*>(index.internalPointer());
        if (node->parent == gridDimensionNode || node == numberOfLayersNode || node == gridTypeNode || node == verticalPinchNode || node == horizontalPinchNode) {
            flags |= Qt::ItemIsEditable;
        }
    }

    return flags;
}

QVariant GridDataModel::headerData(int section, Qt::Orientation orientation, int role) const {
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

QModelIndex GridDataModel::index(int row, int column, const QModelIndex& parent) const {
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

QModelIndex GridDataModel::parent(const QModelIndex& index) const {
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

int GridDataModel::rowCount(const QModelIndex& parent) const {
    TreeNode* parentNode = parent.isValid() ? static_cast<TreeNode*>(parent.internalPointer()) : rootNode;
    return parentNode->children.count();
}

int GridDataModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return 2;
}

int GridDataModel::TreeNode::row() const {
    if (parent) {
        return parent->children.indexOf(const_cast<TreeNode*>(this));
    }
    return 0;
}
