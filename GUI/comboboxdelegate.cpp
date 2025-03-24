#include "comboboxdelegate.h"

ComboBoxDelegate::ComboBoxDelegate(const QStringList& items, QObject* parent) : QStyledItemDelegate(parent), comboItems(items) {}

QWidget* ComboBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex&) const {
    QComboBox* editor = new QComboBox(parent);
    editor->addItems(comboItems); // Добавление элементов в QComboBox
    return editor;
}

void ComboBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
    if (comboBox) {
        QString currentText = index.model()->data(index, Qt::EditRole).toString();
        int idx = comboBox->findText(currentText);
        comboBox->setCurrentIndex(idx >= 0 ? idx : 0); // Если текст найден, установить его; иначе выбрать первый элемент
    }
}

void ComboBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
    if (comboBox) {
        model->setData(index, comboBox->currentText(), Qt::EditRole);
    }
}
