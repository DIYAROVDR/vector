#ifndef DELEGATETYPE_H
#define DELEGATETYPE_H

#include <QMetaType>

// Перечисление для типов делегатов
enum class DelegateType {
    NoDelegate,           // Нет делегата
    DateEditDelegate,     // Делегат для даты
    ComboBoxDelegate,
    IntSpinBoxDelegate,   // Делегат для спинбокса
    DoubleBoxDelegate
};

// Регистрируем перечисление в мета-системе Qt
Q_DECLARE_METATYPE(DelegateType)

#endif // DELEGATETYPE_H
