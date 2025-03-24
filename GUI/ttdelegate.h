#ifndef TTDELEGATE_H
#define TTDELEGATE_H

#include <QItemDelegate>
#include <QPainter>

class ttDelegate : public QItemDelegate {
    Q_OBJECT
public:
    explicit ttDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};
#endif // TTDELEGATE_H
