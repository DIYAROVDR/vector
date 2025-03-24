#ifndef PERFORATIONMODEL_H
#define PERFORATIONMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QDateTime>

#include "../Core/h5filemanager.h"

class PerforationModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit PerforationModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    void setCurrentWell(const QString& wellname);

    bool setData(const QModelIndex &index, const QVariant& value, int role = Qt::EditRole) override;
    bool insertRow(int row, const QModelIndex& parent = QModelIndex());
    bool removeRow(int row, const QModelIndex& parent = QModelIndex());

private:
    void savePerforation();
    void loadPerforation();
private:
    Eigen::Matrix<double, Eigen::Dynamic, 7> perforation;
    QString currentWellName;
    QDateTime startDate;
    H5FileManager& h5filemanager;
};

#endif // PERFORATIONMODEL_H
