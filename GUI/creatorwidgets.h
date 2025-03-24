#ifndef CREATORWIDGETS_H
#define CREATORWIDGETS_H


#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QValidator>
#include <QDateEdit>
#include <QFont>
#include <QObject>

class CreatorWidgets {
public:
    // Метод для создания QLineEdit с валидатором
    static QLineEdit* lineEdit(const QString &text, QValidator *validator = nullptr);

    // Метод для создания QPushButton и связывания с лямбда-функцией
    static QPushButton* pushButton(const QString &text, QWidget* parent = nullptr);

    // Перегруженный метод для создания QPushButton с иконкой
    static QPushButton* pushButton(const QIcon &icon,bool delbackground = true, QWidget* parent = nullptr);

    // Метод для создания QComboBox с переданным списком строк
    static QComboBox* comboBox(const QStringList &items);

    // Метод для создания QSpinBox с минимальным, максимальным значениями и начальным значением
    static QSpinBox* spinBox(int minval, int maxval, int value = 1);

    // Метод для создания QCheckBox с текстом и опционально заданным состоянием
    static QCheckBox* checkBox(const QString &text, bool checked = false);

    static QDateEdit* dateEdit(const QDate &date = QDate::currentDate(), QWidget* parent = nullptr);
};


class ComboValidator : public QValidator {
public:
    ComboValidator(double bottom, double top, QObject* parent = nullptr)
        : QValidator(parent), m_bottom(bottom), m_top(top) {}

    QValidator::State validate(QString& input, int& pos) const override {
        if (input.isEmpty()) {
            return QValidator::Intermediate;
        }

        // Проверяем обычную нотацию
        bool ok;
        double value = input.toDouble(&ok);
        if (ok && value >= m_bottom && value <= m_top) {
            return QValidator::Acceptable;
        }

        // Проверяем научную нотацию
        static const QRegularExpression re("^-?\\d+(\\.\\d+)?([eE][-+]?\\d+)?(,\\s*-?\\d+(\\.\\d+)?([eE][-+]?\\d+)?)*");

        QRegularExpressionMatch match = re.match(input);

        if (match.hasMatch()) {
            value = input.toDouble(&ok);
            if (ok && value >= m_bottom && value <= m_top) {
                return QValidator::Acceptable;
            }
        }

        return QValidator::Invalid;
    }

private:
    double m_bottom;
    double m_top;
};

#endif // CREATORWIDGETS_H
