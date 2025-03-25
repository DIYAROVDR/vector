#include "pvtmodel.h"

PVTModel::PVTModel(QObject* parent):
    QAbstractItemModel(parent), rootNode(new TreeNode("Размерность")) {

    densityNode = new TreeNode("Плотности", rootNode);
    waterDensityNode = new TreeNode("Вода", densityNode);
    oilDensityNode = new TreeNode("Нефть", densityNode);
    gasDensityNode = new TreeNode("Газ", densityNode);

    densityNode->children.append({
         waterDensityNode,
         oilDensityNode,
         gasDensityNode
    });

    waterPropNode = new TreeNode("Свойства воды", rootNode);

    TreeNode* pwNode = new TreeNode("Опорное давление", waterPropNode);
    TreeNode* bwNode = new TreeNode("Коэфициент объемного расширения", waterPropNode);
    TreeNode* cwNode = new TreeNode("Коэфициент сжимаемости", waterPropNode);
    TreeNode* muwNode = new TreeNode("Вязкость", waterPropNode);
    TreeNode* cvwNode = new TreeNode("Градиент вязкости", waterPropNode);

    waterPropNode->children.append({
        pwNode,
        bwNode,
        cwNode,
        muwNode,
        cvwNode
    });

    rootNode->children.append(densityNode);
    rootNode->children.append(waterPropNode);
}


PVTModel::~PVTModel() {}


QModelIndex PVTModel::index(int row, int column, const QModelIndex& parent) const {
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


QModelIndex PVTModel::parent(const QModelIndex& index) const {
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


int PVTModel::rowCount(const QModelIndex& parent) const {
    TreeNode* parentNode = parent.isValid() ? static_cast<TreeNode*>(parent.internalPointer()) : rootNode;
    return parentNode->children.count();
}


int PVTModel::columnCount(const QModelIndex& parent) const {
    return 2;
}


QVariant PVTModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    TreeNode* node = static_cast<TreeNode*>(index.internalPointer());

    switch (role) {
        case Qt::DisplayRole:
            if (index.column() == 0) {
                return node->name;
            }

        case Qt::FontRole:
            if (index.column() == 0 && node->parent == rootNode) {
                QFont font;
                font.setBold(true);
                return font;
            }
            break;
    };

    return QVariant();
}


QVariant PVTModel::headerData(int section, Qt::Orientation orientation, int role) const {
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
