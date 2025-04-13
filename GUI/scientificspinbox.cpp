#include "scientificspinbox.h"

ScientificSpinBox::ScientificSpinBox(QWidget* parent)
        : QDoubleSpinBox(parent),
          m_validator(QRegularExpression(R"([+-]?(\d+\.?\d*|\.\d+)([eE][+-]?\d+)?$)"))
{
    setDecimals(10);  // Больше знаков для научной записи
    setRange(-1e308, 1e308);  // Максимальный диапазон double
}

QValidator::State ScientificSpinBox::validate(QString& input, int& pos) const {
    // Разрешаем пустую строку при редактировании
    if (input.isEmpty()) return QValidator::Intermediate;

    // Проверяем регулярным выражением
    return m_validator.validate(input, pos);
}

double ScientificSpinBox::valueFromText(const QString& text) const {
    if (text.isEmpty()) return 0.0;
    return text.toDouble();  // Qt автоматически обрабатывает 'e'/'E'
}

QString ScientificSpinBox::textFromValue(double val) const {
    // Форматируем в научной нотации при больших/малых числах
    if (qAbs(val) >= 1e6 || (qAbs(val) <= 1e-6 && val != 0.0)) {
        return QLocale::c().toString(val, 'e', decimals());
    }
    return QLocale::c().toString(val, 'f', decimals());
}

void ScientificSpinBox::stepBy(int steps) {
    if (value() == 0.0) {
        setValue(steps * 0.01);
        return;
    }

    double step = std::pow(10, std::floor(std::log10(std::abs(value())))) / 100;
    setValue(value() + steps * step);
}