#ifndef GENERALMODEL_H
#define GENERALMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QVector>
#include <QDateTime>
#include <QFont>
#include <QDebug>
#include <QMap>

#include "delegatetype.h"

#include "../Core/physicalquantity.h"
#include "../Core/h5filemanager.h"


class GeneralModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit GeneralModel(QObject* parent, PhysicalQuantity* physicalquantity);
    ~GeneralModel();

    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    void saveData();
    void loadData();

private:
    QFont bold;
    struct TreeNode {
        QString name;
        QVector<TreeNode*> children;
        TreeNode* parent;
        QVariant value;

        TreeNode(const QString& name, const QVariant& value = QVariant(), TreeNode* parent = nullptr):
                name(name),
                value(value),
                parent(parent) {
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

    TreeNode* rootNode;
    TreeNode* dateNode;
    TreeNode* unitNode;
    TreeNode* phaseNode;
    TreeNode* initNode;
    TreeNode* regionNode;

    H5FileManager& h5filemanager;
    PhysicalQuantity* physicalquantity;

    QDateTime startDate;
    QMap<Grid::Initialization,QString> init;
    QMap<Unit::System, QString> unit;
};

#endif // GENERALMODEL_H
