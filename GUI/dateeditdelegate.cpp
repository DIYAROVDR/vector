#include "dateeditdelegate.h"


DateEditDelegate::DateEditDelegate(QObject *parent): QStyledItemDelegate{parent} {}

QWidget* DateEditDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option , const QModelIndex&) const {
    QDateTimeEdit* editor = new QDateTimeEdit(parent);
    editor->setCalendarPopup(true);
    return editor;
}

void DateEditDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    QDateTimeEdit* dateEdit = qobject_cast<QDateTimeEdit*>(editor);
    if (dateEdit) {
        dateEdit->setDateTime(index.model()->data(index, Qt::EditRole).toDateTime());
    }
}

void DateEditDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    QDateTimeEdit* dateEdit = qobject_cast<QDateTimeEdit*>(editor);
    if (dateEdit) {
        model->setData(index, dateEdit->date(), Qt::EditRole);
    }
}
