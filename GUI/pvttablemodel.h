#ifndef PVTTABLEMODEL_H
#define PVTTABLEMODEL_H

#include <QAbstractTableModel>
#include "../Core/pvtw.h"

class PVTTreeModel; // Предварительное объявление

class PVTTableModel : public QAbstractTableModel {
Q_OBJECT

public:
    explicit PVTTableModel(QObject* parent = nullptr);
    ~PVTTableModel();

    // Стандартные методы модели
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Методы для синхронизации с TreeModel
    void setTreeModel(PVTTreeModel* treeModel);
    void updateData();
    void loadData();

private:
    PVTTreeModel* pvtTreeModel;
    QStringList headers;
    QVector<QVector<double>> tableData;
};

#endif // PVTTABLEMODEL_H