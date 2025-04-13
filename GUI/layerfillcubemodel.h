#ifndef LAYERFILLCUBEMODEL_H
#define LAYERFILLCUBEMODEL_H

#include <QAbstractTableModel>
#include <QDebug>
#include "../GUI/delegatetype.h"
#include "../Core/h5filemanager.h"

class LayerFillCubeModel : public QAbstractTableModel {
public:
    explicit LayerFillCubeModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    void setCurrentCube(const QString& cube);
    void saveCube();
    void setConstValue(int val);
public slots:
    void updateDimens();
private:
    int nx = 1;
    int ny = 1;
    int nz = 1;
    double allLayersVal = 1.0;
    Eigen::ArrayXd layers;
    QString currentCube;
    H5FileManager& h5filemanager;
};

#endif // LAYERFILLCUBEMODEL_H
