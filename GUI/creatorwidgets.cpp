#include "creatorwidgets.h"

// Создание QLineEdit с валидатором
QLineEdit* CreatorWidgets::lineEdit(const QString &text, QValidator* validator) {
    QLineEdit* lineEdit = new QLineEdit(text);
    lineEdit->setValidator(validator);
    lineEdit->setFrame(false);
    lineEdit->setStyleSheet("QLineEdit {background-color: rgba(0, 0, 0, 0);}");

    QFont mainFont;
    mainFont.setPointSize(10);
    lineEdit->setFont(mainFont);
    return lineEdit;
}


// Создание QPushButton и связывание с переданным лямбда-слотом
QPushButton* CreatorWidgets::pushButton(const QString &text, QWidget* parent) {
    QPushButton* btn = new QPushButton(text,parent);
    btn->setFlat(true);
    btn->setStyleSheet("QPushButton {background-color: rgba(0, 0, 0, 0);}");

    QFont mainFont;
    mainFont.setPointSize(10);
    btn->setFont(mainFont);

    return btn;
}

QPushButton* CreatorWidgets::pushButton(const QIcon &icon,bool delbackground, QWidget *parent) {
    QPushButton* btn = new QPushButton(parent);
    btn->setIcon(icon);
    btn->setFlat(true);

    if(delbackground){
        btn->setStyleSheet("QPushButton {background-color: rgba(0, 0, 0, 0);}");
    }

    return btn;
}


// Создание QComboBox с элементами
QComboBox* CreatorWidgets::comboBox(const QStringList &items) {
    QComboBox* comboBox = new QComboBox();

    comboBox->addItems(items);
    comboBox->setFrame(false);
    comboBox->setEditable(true);
    comboBox->lineEdit()->setReadOnly(true);

    QObject::connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [comboBox]() {
        comboBox->lineEdit()->deselect();
    });

    return comboBox;
}

// Создание QSpinBox с заданными минимальным, максимальным и начальным значениями
QSpinBox* CreatorWidgets::spinBox(int minval, int maxval, int value) {
    QSpinBox* spinBox = new QSpinBox();

    QFont mainFont;
    mainFont.setPointSize(10);
    spinBox->setFont(mainFont);
    spinBox->setMinimum(minval);
    spinBox->setMaximum(maxval);
    spinBox->setValue(value);
    spinBox->setFrame(false);

    spinBox->setStyleSheet(
        "QSpinBox {"
        "    background-color: rgba(0, 0, 0, 0);"
        "    border: none;"
        "    padding: 0px;"
        "    margin-left: 5px; margin-right: 5px;"
        "}"
        "QSpinBox::up-button {"
        "    width: 12px;"
        "    height: 12px;"
        "    background-color: rgba(0, 0, 0, 0);"
        "    border: none;"
        "}"
        "QSpinBox::down-button {"
        "    width: 12px;"
        "    height: 12px;"
        "    background-color: rgba(0, 0, 0, 0);"
        "    border: none;"
        "}"
        "QSpinBox::up-arrow {"
        "    image: url(:/images/up-arrow.png);"
        "    width: 8px;"
        "    height: 8px;"
        "}"
        "QSpinBox::down-arrow {"
        "    image: url(:/images/down-arrow.png);"
        "    width: 8px;"
        "    height: 8px;"
        "}"
        );

    return spinBox;
}

// Создание QCheckBox с текстом и заданным состоянием
QCheckBox* CreatorWidgets::checkBox(const QString &text, bool checked) {
    QCheckBox* checkBox = new QCheckBox(text);

    QFont mainFont;
    checkBox->setFont(mainFont);
    checkBox->setChecked(checked);
    checkBox->setStyleSheet("QCheckBox { margin-left: 5px; margin-right: 5px; }");

    return checkBox;
}

QDateEdit *CreatorWidgets::dateEdit(const QDate &date, QWidget *parent) {
    QDateEdit* dateEdit = new QDateEdit(date, parent);
    dateEdit->setCalendarPopup(true); // Включаем всплывающий календарь
    dateEdit->setDisplayFormat("dd.MM.yyyy"); // Формат отображения даты
    dateEdit->setFrame(false);
    return dateEdit;
}
