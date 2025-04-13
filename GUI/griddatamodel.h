#ifndef GRIDDATAMODEL_H
#define GRIDDATAMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QVector>
#include <QDateTime>
#include <QFont>
#include <QDebug>
#include <QMap>

#include "../GUI/delegatetype.h"
#include "../Core/physicalquantity.h"
#include "../Core/h5filemanager.h"

class GridDataModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit GridDataModel(QObject* parent = nullptr);
    ~GridDataModel();

    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    void applyData();

signals:
    void dimensChanged();

private:
    int nx = 1;
    int ny = 1;
    int nz = 1;
    int layers = 1;
    double vertValPinch = 0.0;
    double horValPinch = 0.0;

    struct TreeNode {
        QString name;
        QVector<TreeNode*> children;
        TreeNode* parent;

        TreeNode(const QString& name, TreeNode* parent = nullptr) : name(name), parent(parent) {}
        ~TreeNode() { qDeleteAll(children); }
        int row() const {
            if (parent) {
                return parent->children.indexOf(const_cast<TreeNode*>(this));
            }
            return 0;
        }
    };

    TreeNode* rootNode;
    TreeNode* gridDimensionNode;
    TreeNode* nxNode;
    TreeNode* nyNode;
    TreeNode* nzNode;
    TreeNode* layersNode;
    TreeNode* gridTypeNode;
    TreeNode* verticalPinchNode;
    TreeNode* horizontalPinchNode;

    H5FileManager& h5filemanager;

    Grid::Type currentTypeGrid = Grid::Type::BLOCKCENTERED;
    QMap<Grid::Type, QString> gridTypeNames;
};

#endif // GRIDDATAMODEL_H
