#include "pvttablemodel.h"
#include "pvttreemodel.h"

PVTTableModel::PVTTableModel(QObject* parent) :
            QAbstractTableModel(parent),
            pvtTreeModel(nullptr) {
}

PVTTableModel::~PVTTableModel() {}

void PVTTableModel::setTreeModel(PVTTreeModel* treeModel) {
    beginResetModel();
    pvtTreeModel = treeModel;
    // Подключаем сигнал об изменении данных в TreeModel к обновлению TableModel
    if (pvtTreeModel) {
        connect(pvtTreeModel, &PVTTreeModel::dataChanged, this, &PVTTableModel::updateData);
        connect(pvtTreeModel, &PVTTreeModel::currentPhaseChanged, this, &PVTTableModel::loadData);
    }
    endResetModel();
}

void PVTTableModel::updateData() {
    if (!pvtTreeModel) return;

    beginResetModel();

    endResetModel();
}

int PVTTableModel::rowCount(const QModelIndex& parent) const {
    if (!pvtTreeModel || parent.isValid() || tableData.size() == 0) return 0;
    return tableData.first().size(); // Примерное количество строк (давлений)
}

int PVTTableModel::columnCount(const QModelIndex& parent) const {
    if (!pvtTreeModel || parent.isValid() || headers.size() == 0) return 0;
    return headers.size();
}

QVariant PVTTableModel::data(const QModelIndex& index, int role) const {
    if (!pvtTreeModel || !index.isValid()) return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return tableData[index.column()][index.row()];
    }

    return QVariant();
}

QVariant PVTTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal && headers.size() !=0) {
        return headers.at(section);
    }
    return QVariant();
}

bool PVTTableModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!pvtTreeModel || !index.isValid() || role != Qt::EditRole) return false;

    switch(role) {

    };

    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags PVTTableModel::flags(const QModelIndex& index) const {
    if (!index.isValid()) return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void PVTTableModel::loadData() {
    beginResetModel();  // Начинаем сброс модели
    headers = pvtTreeModel->tableHeaders();  // Обновляем заголовки
    tableData = pvtTreeModel->tableData();
    endResetModel();    // Завершаем сброс и уведомляем представление
    emit dataChanged(createIndex(0,0), createIndex(tableData.first().size(),tableData.size()));

    emit headerDataChanged(Qt::Horizontal, 0, headers.size());
}
