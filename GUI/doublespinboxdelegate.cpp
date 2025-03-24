#include "doublespinboxdelegate.h"

DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(double maxval,double minval,double step,int decimal,QObject* parent):
                    QStyledItemDelegate(parent),
                    maxval(maxval),
                    minval(minval),
                    step(step),
                    decimal(decimal) {
}

QWidget *DoubleSpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &,const QModelIndex &) const {
    QDoubleSpinBox* editor = new QDoubleSpinBox(parent);
    editor->setMinimum(minval);
    editor->setMaximum(maxval);
    editor->setDecimals(decimal);
    editor->setSingleStep(step);
    return editor;
}

void DoubleSpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    double value = index.model()->data(index, Qt::EditRole).toDouble();
    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox *>(editor);
    if (spinBox) {
        spinBox->setValue(value);
    }
}

void DoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const {
    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox *>(editor);
    if (spinBox) {
        model->setData(index, spinBox->value(), Qt::EditRole);
    }
}
