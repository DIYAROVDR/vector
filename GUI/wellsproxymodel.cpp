#include "wellsproxymodel.h"

WellsProxyModel::WellsProxyModel(QObject *parent) : QSortFilterProxyModel{parent} {}

bool WellsProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    // Получаем индекс строки
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    // Приводим модель к типу QStandardItemModel для доступа к itemFromIndex
    QStandardItemModel* standardModel = qobject_cast<QStandardItemModel*>(sourceModel());
    if (!standardModel) {
        return false;  // Если это не QStandardItemModel, ничего не отображаем
    }

    // Получаем элемент из модели
    QStandardItem* item = standardModel->itemFromIndex(index);
    if (item && item->parent() == nullptr) {
        return true;  // Родительский элемент "Скважины" всегда видим
    }

    // Проверяем текст элемента на соответствие фильтру
    QString itemText = sourceModel()->data(index).toString();
    return itemText.contains(filterRegExp());
}
