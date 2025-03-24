#ifndef WELLTRAJECTORYMODEL_H
#define WELLTRAJECTORYMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QDebug>

#include "../Core/h5filemanager.h"

class WellTrajectoryModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit WellTrajectoryModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    void setCurrentWell(const QString& wellname);

    bool insertRow(int row, const QModelIndex& parent = QModelIndex());
    bool removeRow(int row, const QModelIndex& parent = QModelIndex());

    void saveWellTrack();
    void loadWellTrack();
private:
    Eigen::Matrix<double, Eigen::Dynamic, 4> welltrack;
    H5FileManager& h5filemanager;
    QString currentWellName;
};

#endif // WELLTRAJECTORYMODEL_H
