#ifndef WELLSPROXYMODEL_H
#define WELLSPROXYMODEL_H

#include <QStandardItemModel>
#include <QSortFilterProxyModel>

class WellsProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit WellsProxyModel(QObject* parent = nullptr);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;
};

#endif // WELLSPROXYMODEL_H
