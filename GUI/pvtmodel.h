#ifndef PVTMODEL_H
#define PVTMODEL_H

#include <QAbstractItemModel>
#include <QFont>


class PVTModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit PVTModel(QObject* parent = nullptr);
    ~PVTModel();

    // Переопределение необходимых методов QAbstractItemModel
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
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
    TreeNode* densityNode;
    TreeNode* waterDensityNode;
    TreeNode* oilDensityNode;
    TreeNode* gasDensityNode;
    TreeNode* waterPropNode;
};

#endif // PVTMODEL_H
