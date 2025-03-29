#include "multidelegate.h"


MultiDelegate::MultiDelegate(QObject *parent): QStyledItemDelegate{parent} {}

QWidget* MultiDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    DelegateType delegateType = index.data(Qt::UserRole + 1).value<DelegateType>();

    switch (delegateType) {
    case DelegateType::DateEditDelegate: {
        QDateTimeEdit* editor = new QDateTimeEdit(parent);
        editor->setCalendarPopup(true);
        editor->setDisplayFormat("dd.MM.yyyy");
        return editor;
    }
    case DelegateType::IntSpinBoxDelegate: {
        QSpinBox* editor = new QSpinBox(parent);
        editor->setMinimum(0);
        editor->setMaximum(100);
        return editor;
    }
    case DelegateType::ComboBoxDelegate: {
        //index // как то получить итемы которые должны отображаться.
        QComboBox* editor = new QComboBox(parent);
        QStringList items = index.data(Qt::UserRole + 2).toStringList();
        editor->addItems(items);
        return editor;
    }
    case DelegateType::DoubleBoxDelegate: {
        QDoubleSpinBox* editor = new QDoubleSpinBox(parent);
        editor->setMinimum(0.0);
        editor->setMaximum(100.0);
        editor->setDecimals(2);
        return editor;
    }

    case DelegateType::ScientificNotationDelegate: {
        ScientificSpinBox* editor = new ScientificSpinBox(parent);
        editor->setMinimum(-1e100);  // Широкий диапазон
        editor->setMaximum(1e100);

        editor->setDecimals(6);      // Больше знаков после запятой
        return editor;
    }

    default:
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void MultiDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    DelegateType delegateType = index.data(Qt::UserRole + 1).value<DelegateType>();

    switch (delegateType) {
    case DelegateType::DateEditDelegate: {
        QDateTimeEdit* dateEdit = qobject_cast<QDateTimeEdit*>(editor);
        if (dateEdit) {
            dateEdit->setDateTime(index.data(Qt::EditRole).toDateTime());
        }
        break;
    }
    case DelegateType::IntSpinBoxDelegate: {
        QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
        if (spinBox) {
            spinBox->setValue(index.data(Qt::EditRole).toInt());
        }
        break;
    }
    case DelegateType::ComboBoxDelegate: {
        QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
        if (comboBox) {
            comboBox->setCurrentText(index.data(Qt::EditRole).toString());
        }
        break;
    }
    case DelegateType::DoubleBoxDelegate: {
        QDoubleSpinBox* doubleSpinBox = qobject_cast<QDoubleSpinBox*>(editor);
        if (doubleSpinBox) {
            doubleSpinBox->setValue(index.data(Qt::EditRole).toDouble());
        }
        break;
    }
    case DelegateType::ScientificNotationDelegate: {
        ScientificSpinBox* spinBox = qobject_cast<ScientificSpinBox*>(editor);
        if (spinBox) {
            spinBox->setValue(index.data(Qt::EditRole).toDouble());
        }
        break;
    }
    default:
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void MultiDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    DelegateType delegateType = index.data(Qt::UserRole + 1).value<DelegateType>();

    switch (delegateType) {
    case DelegateType::DateEditDelegate: {
        QDateTimeEdit* dateEdit = qobject_cast<QDateTimeEdit*>(editor);
        if (dateEdit) {
            model->setData(index, dateEdit->dateTime(), Qt::EditRole);
        }
        break;
    }
    case DelegateType::IntSpinBoxDelegate: {
        QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
        if (spinBox) {
            model->setData(index, spinBox->value(), Qt::EditRole);
        }
        break;
    }
    case DelegateType::ComboBoxDelegate: {
        QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
        if (comboBox) {
            model->setData(index, comboBox->currentText(), Qt::EditRole);
        }
        break;
    }
    case DelegateType::DoubleBoxDelegate: {
        QDoubleSpinBox* doubleSpinBox = qobject_cast<QDoubleSpinBox*>(editor);
        if (doubleSpinBox) {
            model->setData(index, doubleSpinBox->value(), Qt::EditRole);
        }
        break;
    }
    case DelegateType::ScientificNotationDelegate: {
        ScientificSpinBox* spinBox = qobject_cast<ScientificSpinBox*>(editor);
        if (spinBox) {
            model->setData(index, spinBox->value(), Qt::EditRole);
        }
        break;
    }
    default:
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void MultiDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    Q_ASSERT(index.isValid());
    QStyleOptionViewItem newOption(option);

    QRect fullRect = option.rect;
    fullRect.setLeft(0);
    painter->save();

    if (option.state & QStyle::State_Selected) {
        QPen pen(QColor(0, 122, 204));
        pen.setWidth(3);
        painter->setPen(pen);
    } else {
        QPen pen(option.palette.color(QPalette::Midlight));
        pen.setWidth(1);
        painter->setPen(pen);
    }

    painter->drawRect(fullRect);
    painter->restore();

    QStyledItemDelegate::paint(painter, newOption, index);
}
