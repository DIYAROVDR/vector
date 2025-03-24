#ifndef PVTMODEL_H
#define PVTMODEL_H

#include <QAbstractItemModel>

class PVTModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit PVTModel(QObject* parent = nullptr);
    ~PVTModel();

    // Переопределение необходимых методов QAbstractItemModel
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
         // Здесь вы можете добавить свои данные и методы для работы с ними
};

#endif // PVTMODEL_H
