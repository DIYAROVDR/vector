#include "layerfillcubemodel.h"

LayerFillCubeModel::LayerFillCubeModel(QObject* parent):
                    QAbstractTableModel{parent},
                    h5filemanager(H5FileManager::instance()) {
    layers.resize(1);
    layers.setConstant(1.0);
}


int LayerFillCubeModel::rowCount(const QModelIndex& parent) const {
    return nz + 1; // +1 для строки "Все слои"
}


int LayerFillCubeModel::columnCount(const QModelIndex& parent) const {
    return 2;
}


QVariant LayerFillCubeModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    int row = index.row();
    int col = index.column();

    switch (role) {
    case Qt::DisplayRole:
        if (row == 0) { // Первая строка — "Все слои"
            if (col == 0) {
                return "Все слои";
            } else {
                return allLayersVal; // Значение для "Все слои"
            }
        } else { // Остальные строки
            if (col == 0) {
                return row; // Номер слоя
            } else {
                return layers[row - 1]; // Значение для слоя
            }
        }
    case Qt::EditRole:
        if (col == 1) {
            return row == 0 ? allLayersVal : layers[row - 1]; // Возвращаем значение для редактирования
        }
        break;
    case Qt::UserRole + 1:
        if (col == 1) {
            return QVariant::fromValue(DelegateType::DoubleBoxDelegate);
        }
        break;
    }

    return QVariant();
}


QVariant LayerFillCubeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0: return "Слой";
            case 1: return QString("Значение [ %1 ]").arg(currentCube);
            }
        }
    }
    return QVariant();
}


Qt::ItemFlags LayerFillCubeModel::flags(const QModelIndex& index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    if(index.column() == 0){
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}


bool LayerFillCubeModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid() || index.column() == 0) {
        return false;
    }

    if (role == Qt::EditRole) {
        int row = index.row();
        double newValue = value.toDouble();

        if (row == 0) { // Если редактируется строка "Все слои"
            allLayersVal = newValue;
            layers.setConstant(newValue); // Применяем значение ко всем слоям
            emit dataChanged(this->index(0, 1), this->index(nz + 1, 1));
        } else {
            layers[row - 1] = newValue;
            if(nz == 1) {
                allLayersVal = newValue;
            }
            emit dataChanged(index, index);
        }

        return true;
    }

    return false;
}


void LayerFillCubeModel::setCurrentCube(const QString& cube) {
    currentCube = cube;
    emit headerDataChanged(Qt::Horizontal, 0, 1);
}


void LayerFillCubeModel::saveCube() {
    if (currentCube.isEmpty()) {
        return;
    }
    Eigen::VectorXd cube(nx * ny * nz);

    for (int k = 0; k < nz; ++k) {
        QModelIndex index = this->index(k + 1 , 1);

        QVariant value = data(index, Qt::EditRole);

        if (!value.isValid() || value.isNull()) {
            return;
        }

        double val = value.toDouble();

        int startIndex = k * nx * ny;
        cube.segment(startIndex, nx * ny).setConstant(val);
    }

    h5filemanager.saveStaticCube(cube, currentCube.toStdString());
}


void LayerFillCubeModel::updateDimens() {
    Eigen::Array3i dim = h5filemanager.getDimens();
    int nzold = nz;
    nx = dim[0];
    ny = dim[1];
    nz = dim[2];
    layers.resize(nz); // Изменяем размер layers на nz (без учета строки "Все слои")
    layers.setConstant(allLayersVal); // Устанавливаем значение всех слоев

    if (nzold == nz) {
        return;
    } else if (nz > nzold) {
        // Добавляем новые строки
        beginInsertRows(QModelIndex(), nzold + 1, nz); // +1 для строки "Все слои"
        endInsertRows();
    } else if (nz < nzold) {
        // Удаляем лишние строки
        beginRemoveRows(QModelIndex(), nz + 1, nzold); // +1 для строки "Все слои"
        endRemoveRows();
    }
}

