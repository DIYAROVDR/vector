#include "wellcontrolmodel.h"

WellControlModel::WellControlModel(QObject* parent, PhysicalQuantity* physicalQuantity):
                  QAbstractTableModel(parent),
                  physicalQuantity(physicalQuantity),
                  h5filemanager(H5FileManager::instance()) {
    startDate = QDateTime::fromString(QString::fromStdString(h5filemanager.startDate()), Qt::ISODate);
}

int WellControlModel::rowCount(const QModelIndex& /*parent*/) const {
    return controls.rows();
}

int WellControlModel::columnCount(const QModelIndex& /*parent*/) const {
    return 3; // Три столбца: Дата, Тип граничного условия, Значение
}

QVariant WellControlModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= controls.rows()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: return startDate.addSecs(controls(index.row(), 0)).toString("dd.MM.yyyy");  // Дата
        case 1: return QString::fromStdString(physicalQuantity->controlName(static_cast<PhysicalQuantity::Quantity>(controls(index.row(), 1)))); // Тип граничного условия
        case 2: return controls(index.row(), 2);  // Значение
        }
    } else if (role == Qt::EditRole) {
        switch (index.column()) {
        case 0: return startDate.addSecs(controls(index.row(), 0));  // DateTime
        case 1: return QString::fromStdString(physicalQuantity->controlName(static_cast<PhysicalQuantity::Quantity>(controls(index.row(), 1))));
        case 2: return controls(index.row(), 2);  // Значение
        }
    }

    return QVariant();
}

QVariant WellControlModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0: return "Дата";
            case 1: return "Тип граничного условия";
            case 2: return "Значение";
            }
        }
    }
    return QVariant();
}

Qt::ItemFlags WellControlModel::flags(const QModelIndex& index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

bool WellControlModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid() || role != Qt::EditRole) {
        return false;
    }

    switch (index.column()) {
    case 0: {
        QDateTime date = QDateTime::fromString(value.toString(), Qt::ISODate);
        controls(index.row(), 0) = startDate.secsTo(date);  // Разница в секундах
        break;
    }
    case 1:
        // Тип граничного условия
        controls(index.row(), 1) = static_cast<int>(physicalQuantity->controlType(value.toString().toStdString()));
        break;
    case 2: controls(index.row(), 2) = value.toDouble(); break;  // Значение
    default: return false;
    }

    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

    // Автоматическое сохранение
    saveControls();
    return true;
}


bool WellControlModel::insertRow(int row, const QModelIndex &parent) {
    if (row < 0 || row > controls.rows()) {
        return false;
    }

    beginInsertRows(parent, row, row);
    controls.conservativeResize(controls.rows() + 1, 3);
    controls.row(row).setZero();  // Инициализация новой строки нулями
    endInsertRows();

    // Автоматическое сохранение
    saveControls();
    return true;
}


bool WellControlModel::removeRow(int row, const QModelIndex& parent) {
    if (row < 0 || row >= controls.rows()) {
        return false;
    }

    beginRemoveRows(parent, row, row);
    Eigen::Matrix<double, Eigen::Dynamic, 3> newControls(controls.rows() - 1, 3);
    newControls << controls.topRows(row), controls.bottomRows(controls.rows() - row - 1);
    controls = newControls;
    endRemoveRows();

    // Автоматическое сохранение
    saveControls();
    return true;
}


void WellControlModel::setCurrentWell(const QString& currentWellName) {
    this->currentWellName = currentWellName;
    loadControls();
    emit layoutChanged();
}


QStringList WellControlModel::controlItems() {
    std::vector<std::string> items = physicalQuantity->controlItems();
    list.reserve(items.size());
    std::transform(std::begin(items), std::end(items), std::back_inserter(list),&QString::fromStdString);
    return list;
}


void WellControlModel::saveControls() {
    if (!currentWellName.isEmpty()) {
        h5filemanager.saveControls(currentWellName.toStdString(), controls);
    }
}


void WellControlModel::loadControls() {
    if (!currentWellName.isEmpty()) {
        controls = h5filemanager.loadControls(currentWellName.toStdString());
        emit layoutChanged();
    }
}
