#ifndef DATEEDITDELEGATE_H
#define DATEEDITDELEGATE_H

#include <QStyledItemDelegate>
#include <QDateEdit>
#include <QPainter>

class DateEditDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit DateEditDelegate(QObject* parent = nullptr);
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem &, const QModelIndex &) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
};


#endif // DATEEDITDELEGATE_H
