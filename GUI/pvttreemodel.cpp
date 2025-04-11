#include "pvttreemodel.h"

PVTTreeModel::PVTTreeModel(PhysicalQuantity* physicalquantity, QObject* parent):
        quantity(physicalquantity),
        QAbstractItemModel(parent),
        rootNode(new TreeNode("PVT")) {

    bold.setBold(true);

    regionsNode = new TreeNode("Регионы", rootNode);
    firstRegNode = new TreeNode("DV", regionsNode, 1, Unit::Types::UNDEFINED, true);

    regionsNode->children.append(firstRegNode);

    waterNode = new TreeNode("Свойства воды", rootNode);

    std::vector<double> data = {1E-9, 1000.0,200E+5, 1E-3, 0.0, 1.0}; // SI

    int count = 0;
    for(const auto& pair : physicalquantity->getPVTDataNames()) {
        waterNode->children.append(new TreeNode(
                QString::fromStdString(pair.second),
                waterNode,
                physicalquantity->diconvert(data[count], pair.first), // SI -> TS
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
    int row = lastSelectNode->row();
    switch (role) {
        case Qt::DisplayRole:
            if (index.column() == 0) {
                return node->name;
            }
             if (node->parent != rootNode) {
                return node->value;
            }
        case Qt::EditRole:
            if (node->parent == regionsNode) {
                return node->value;
            }
            if (node->parent == waterNode) {
                //return node->value;
                switch (node->type) {
                    case Unit::Types::PRESSURE:
                        return quantity->diconvert( waterpvt[row]->getPwRef(), node->type);
                    case Unit::Types::VOLUMETRIC_EXPANSION:
                        return quantity->diconvert( waterpvt[row]->getBwRef(), node->type);
                    case Unit::Types::COMPRESSIBILITY:
                        return quantity->diconvert( waterpvt[row]->getCw(), node->type);
                    case Unit::Types::VISCOSITY:
                        return quantity->diconvert( waterpvt[row]->getMwRef(), node->type);
                    case Unit::Types::VISCOSITY_GRADIENT:
                        return quantity->diconvert( waterpvt[row]->getCvw(), node->type);
                    case Unit::Types::DENSITY:
                        return quantity->diconvert( waterpvt[row]->getRhoW(), node->type);
                };
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
    int row = lastSelectNode->row();

    switch(role) {
        case Qt::EditRole :
            if (node->parent == waterNode) {
                node->value = value.toDouble();

                double convval = quantity->convert( node->value, node->type);

                switch (node->type) {
                    case Unit::Types::PRESSURE:
                        waterpvt[row]->setPwRef(convval);
                        break;
                    case Unit::Types::VOLUMETRIC_EXPANSION:
                        waterpvt[row]->setBwRef(convval);
                        break;
                    case Unit::Types::COMPRESSIBILITY:
                        waterpvt[row]->setCw(convval);
                        break;
                    case Unit::Types::VISCOSITY:
                        waterpvt[row]->setMwRef(convval);
                        break;
                    case Unit::Types::VISCOSITY_GRADIENT:
                        waterpvt[row]->setCvw(convval);
                        break;
                    case Unit::Types::DENSITY:
                        waterpvt[row]->setRhoW(convval);
                        break;
                };
                waterpvt[row]->update();

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
    int row = lastSelectNode->row();

    Eigen::ArrayXd muwArray = waterpvt[row]->mutab();
    Eigen::ArrayXd bwArray = waterpvt[row]->btab();
    Eigen::ArrayXd pwArray = waterpvt[row]->ptab();

    std::cout<< bwArray <<std::endl;

    auto muw = quantity->diconvert(muwArray, PhysicalQuantity::Types::VISCOSITY);
    auto bw = quantity->diconvert(bwArray, PhysicalQuantity::Types::VOLUMETRIC_EXPANSION);
    auto pw = quantity->diconvert(pwArray, PhysicalQuantity::Types::PRESSURE);

    QVector<QVector<double>> result = {
        QVector(pw.begin(), pw.end()),
        QVector(muw.begin(), muw.end()),
        QVector(bw.begin(), bw.end())
    };

    return result;
}

