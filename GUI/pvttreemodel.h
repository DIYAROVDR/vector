#ifndef PVTTREEMODEL_H
#define PVTTREEMODEL_H

#include <QAbstractItemModel>
#include <QFont>
#include <QColor>
#include "delegatetype.h"
#include <iostream>

#include "../Core/physicalquantity.h"
#include "../Core/pvtw.h"


class PVTTreeModel : public QAbstractItemModel {
    Q_OBJECT
public:
    explicit PVTTreeModel(PhysicalQuantity* physicalquantity, QObject* parent = nullptr);
    ~PVTTreeModel();

    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QStringList tableHeaders();
    QVector<QVector<double>> tableData();

signals:
    void pvtDataChanged(); // Новый сигнал
    void currentPhaseChanged();
private:
    QFont bold;
    struct TreeNode {
        QString name;
        bool select;
        double value;
        Unit::Types type;
        QVector<TreeNode*> children;
        TreeNode* parent;

        TreeNode(
                const QString& name,
                TreeNode* parent = nullptr,
                double value = 0.0,
                Unit::Types type = Unit::Types::UNDEFINED,
                bool select = false):
                name(name),
                parent(parent),
                value(value),
                type(type),
                select(select) {
        }

        ~TreeNode() {
            qDeleteAll(children);
        }

        int row() const {
            if (parent) {
                return parent->children.indexOf(const_cast<TreeNode*>(this));
            }
            return 0;
        }
    };

    PhysicalQuantity* quantity;

    TreeNode* rootNode;
    TreeNode* regionsNode;
    TreeNode* firstRegNode;
    TreeNode* waterNode;
    TreeNode* lastSelectNode;

    QVector<PVTW*> waterpvt;
};

#endif // PVTTREEMODEL_H
