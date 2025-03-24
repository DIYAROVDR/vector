#include "ttdelegate.h"

void ttDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    Q_ASSERT(index.isValid());
    QStyleOptionViewItem newOption(option);

    QRect fullRect = option.rect;

    fullRect.setLeft(0);

    painter->save();

    QPen pen;
    pen.setColor(option.palette.color(QPalette::Midlight));
    pen.setWidth(1);

    painter->setPen(pen);
    painter->drawRect(fullRect);

    painter->restore();

    QItemDelegate::paint(painter, newOption, index);
}

