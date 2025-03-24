#include "doublevalidatordelegate.h"

DoubleValidatorDelegate::DoubleValidatorDelegate(QObject* parent) : QItemDelegate{parent} {}

QWidget* DoubleValidatorDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QLineEdit* editor = new QLineEdit(parent);
    QDoubleValidator* validator = new QDoubleValidator(0, 1E+10, 5, editor);
    editor->setValidator(validator);
    return editor;
}
