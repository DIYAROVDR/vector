#ifndef CUBESTREEMODEL_H
#define CUBESTREEMODEL_H

#include <QAbstractItemModel>
#include <QVector>
#include <QString>
#include <QVariant>
#include <QModelIndex>
#include <Qt>
#include <QFont>
#include <QPalette>
#include <QIcon>

#include "../Core/physicalquantity.h"

class CubesTreeModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit CubesTreeModel(QObject* parent, PhysicalQuantity* physicalquantity);
    ~CubesTreeModel();

    // Переопределение методов QAbstractItemModel
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    bool insertRow(int row, const QModelIndex& parent = QModelIndex());
    bool removeRow(int row, const QModelIndex& parent = QModelIndex());
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    int itemLevel(const QModelIndex& index) const;

private:
    QFont bold;

    struct TreeNode {
        QString name;
        QVector<TreeNode*> children;
        TreeNode* parent;
        PhysicalQuantity::Quantity value;
        bool isEditable;
        bool select = false;
        bool isEmpty = true;

        TreeNode(const QString& name, TreeNode* parent, PhysicalQuantity::Quantity value, bool isEditable = false):
                name(name),
                parent(parent),
                value(value),
                isEditable(isEditable) {}
        ~TreeNode() { qDeleteAll(children); }

        int row() const {
            if (parent)
                return parent->children.indexOf(const_cast<TreeNode*>(this));
            return 0;
        }
    };

    TreeNode* rootNode;
    TreeNode* initialCubesNode;
    TreeNode* regionsNode;
    TreeNode* userCubesNode;

    TreeNode* lastSelectNode = nullptr;
    PhysicalQuantity* physicalquantity;
};

#endif // CUBESTREEMODEL_H
