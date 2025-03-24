#ifndef WELLCONTROLMODEL_H
#define WELLCONTROLMODEL_H

#include <QAbstractTableModel>
#include <QDateTime>
#include <QString>

#include "../Core/h5filemanager.h"
#include "../Core/physicalquantity.h"

class WellControlModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit WellControlModel(QObject* parent, PhysicalQuantity* physicalQuantity);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    bool insertRow(int row, const QModelIndex& parent = QModelIndex());
    bool removeRow(int row, const QModelIndex& parent = QModelIndex());

    void setCurrentWell(const QString& wellname);

    QStringList controlItems();

private:
    void saveControls();
    void loadControls();

private:
    Eigen::Matrix<double, Eigen::Dynamic, 3> controls;
    H5FileManager& h5filemanager;
    PhysicalQuantity* physicalQuantity;
    QStringList list;
    QString currentWellName;
    QDateTime startDate;
};

#endif // WELLCONTROLMODEL_H
