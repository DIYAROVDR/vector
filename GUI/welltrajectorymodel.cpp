#include "welltrajectorymodel.h"

WellTrajectoryModel::WellTrajectoryModel(QObject* parent):
                    QAbstractTableModel(parent),
                    h5filemanager(H5FileManager::instance()) {
}


int WellTrajectoryModel::rowCount(const QModelIndex & /*parent*/) const {
    return welltrack.rows();
}


int WellTrajectoryModel::columnCount(const QModelIndex & /*parent*/) const {
    return 4; // Четыре столбца: X, Y, Z (TVD), Z (MD)
}


QVariant WellTrajectoryModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= welltrack.rows()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return welltrack(index.row(), index.column());
    }

    return QVariant();
}


QVariant WellTrajectoryModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0: return "Координата по X, м";
            case 1: return "Координата по Y, м";
            case 2: return "Координата по Z (TVD), м";
            case 3: return "Координата по Z (MD), м";
            }
        }
    }
    return QVariant();
}


Qt::ItemFlags WellTrajectoryModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}


bool WellTrajectoryModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || role != Qt::EditRole){
        return false;
    }

    welltrack(index.row(), index.column()) = value.toDouble();
    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
    saveWellTrack();
    return true;
}


void WellTrajectoryModel::setCurrentWell(const QString &wellname) {
    currentWellName = wellname;
    loadWellTrack();
    emit layoutChanged();
}



bool WellTrajectoryModel::insertRow(int row, const QModelIndex &parent) {
    if (row < 0 || row > welltrack.rows()) {
        return false;
    }

    beginInsertRows(parent, row, row);
    welltrack.conservativeResize(welltrack.rows() + 1, 4);
    welltrack.row(row).setZero();
    endInsertRows();
    saveWellTrack();
    return true;
}


bool WellTrajectoryModel::removeRow(int row, const QModelIndex &parent) {
    if (row < 0 || row >= welltrack.rows()) {
        return false;
    }

    beginRemoveRows(parent, row, row);
    Eigen::Matrix<double, Eigen::Dynamic, 4> newWelltrack(welltrack.rows() - 1, 4);
    newWelltrack << welltrack.topRows(row), welltrack.bottomRows(welltrack.rows() - row - 1);
    welltrack = newWelltrack;
    endRemoveRows();
    saveWellTrack();
    return true;
}


void WellTrajectoryModel::saveWellTrack() {
    if (!currentWellName.isEmpty()) {
        h5filemanager.saveWellTrack(currentWellName.toStdString(), welltrack);
    }
}


void WellTrajectoryModel::loadWellTrack() {
    if (!currentWellName.isEmpty()) {
        welltrack = h5filemanager.loadWelltrack(currentWellName.toStdString());
        emit layoutChanged();
    }
}
