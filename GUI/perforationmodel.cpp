#include "perforationmodel.h"

PerforationModel::PerforationModel(QObject* parent):
                  QAbstractTableModel(parent),
                  h5filemanager(H5FileManager::instance()) {
    startDate = QDateTime::fromString(QString::fromStdString(h5filemanager.startDate()), Qt::ISODate);
}


int PerforationModel::rowCount(const QModelIndex& /*parent*/) const {
    return perforation.rows();
}


int PerforationModel::columnCount(const QModelIndex& /*parent*/) const {
    return 7;  // 7 столбцов: Дата открытия, Дата закрытия, Кровля, Подошва, Радиус, Скин-фактор, Мультипликатор
}


QVariant PerforationModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= perforation.rows()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: return startDate.addSecs(perforation(index.row(), 0)).toString("dd.MM.yyyy");  // Дата открытия (строка)
        case 1: return startDate.addSecs(perforation(index.row(), 1)).toString("dd.MM.yyyy");  // Дата закрытия (строка)
        case 2: return perforation(index.row(), 2);  // Кровля
        case 3: return perforation(index.row(), 3);  // Подошва
        case 4: return perforation(index.row(), 4);  // Радиус скважины
        case 5: return perforation(index.row(), 5);  // Скин-фактор
        case 6: return perforation(index.row(), 6);  // Мультипликатор
        }
    } else if (role == Qt::EditRole) {
        switch (index.column()) {
        case 0: return startDate.addSecs(perforation(index.row(), 0));  // Дата открытия (QDateTime)
        case 1: return startDate.addSecs(perforation(index.row(), 1));  // Дата закрытия (QDateTime)
        case 2: return perforation(index.row(), 2);  // Кровля
        case 3: return perforation(index.row(), 3);  // Подошва
        case 4: return perforation(index.row(), 4);  // Радиус скважины
        case 5: return perforation(index.row(), 5);  // Скин-фактор
        case 6: return perforation(index.row(), 6);  // Мультипликатор
        }
    }

    return QVariant();
}

QVariant PerforationModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0: return "Дата открытия";
            case 1: return "Дата закрытия";
            case 2: return "Кровля";
            case 3: return "Подошва";
            case 4: return "Радиус скважины";
            case 5: return "Скин-фактор";
            case 6: return "Мультипликатор";
            }
        }
    }
    return QVariant();
}

Qt::ItemFlags PerforationModel::flags(const QModelIndex& index) const {
    if (!index.isValid()){
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

bool PerforationModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || role != Qt::EditRole) {
        return false;
    }

    switch (index.column()) {
    case 0: {
        QDateTime openingDate = QDateTime::fromString(value.toString(), Qt::ISODate);
        perforation(index.row(), 0) = startDate.secsTo(openingDate);  // Разница в секундах
        break;
    }
    case 1: {
        QDateTime closingDate = QDateTime::fromString(value.toString(), Qt::ISODate);
        perforation(index.row(), 1) = startDate.secsTo(closingDate);  // Разница в секундах
        break;
    }
    case 2: perforation(index.row(), 2) = value.toDouble(); break;  // Кровля
    case 3: perforation(index.row(), 3) = value.toDouble(); break;  // Подошва
    case 4: perforation(index.row(), 4) = value.toDouble(); break;  // Радиус скважины
    case 5: perforation(index.row(), 5) = value.toDouble(); break;  // Скин-фактор
    case 6: perforation(index.row(), 6) = value.toDouble(); break;  // Мультипликатор
    default: return false;
    }

    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

    // Автоматическое сохранение
    savePerforation();
    return true;
}

void PerforationModel::setCurrentWell(const QString &wellname) {
    currentWellName = wellname;
    loadPerforation();
    emit layoutChanged();
}

bool PerforationModel::insertRow(int row, const QModelIndex& parent) {
    if (row < 0 || row > perforation.rows()) {
        return false;
    }

    beginInsertRows(parent, row, row);
    perforation.conservativeResize(perforation.rows() + 1, 7);
    perforation.row(row).setZero();  // Инициализация новой строки нулями
    endInsertRows();

    // Автоматическое сохранение
    savePerforation();
    return true;
}

bool PerforationModel::removeRow(int row, const QModelIndex& parent) {
    if (row < 0 || row >= perforation.rows()) {
        return false;
    }

    beginRemoveRows(parent, row, row);
    Eigen::Matrix<double, Eigen::Dynamic, 7> newPerforation(perforation.rows() - 1, 7);
    newPerforation << perforation.topRows(row), perforation.bottomRows(perforation.rows() - row - 1);
    perforation = newPerforation;
    endRemoveRows();

    // Автоматическое сохранение
    savePerforation();
    return true;
}


void PerforationModel::savePerforation() {
    if (!currentWellName.isEmpty()) {
        h5filemanager.savePerforation(currentWellName.toStdString(), perforation);
    }
}


void PerforationModel::loadPerforation() {
    if (!currentWellName.isEmpty()) {
        perforation = h5filemanager.loadPerforation(currentWellName.toStdString());
        emit layoutChanged();
    }
}
