#ifndef SCIENTIFICSPINBOX_H
#define SCIENTIFICSPINBOX_H

#include <QDoubleSpinBox>
#include <QLocale>
#include <QRegularExpressionValidator>
#include <cmath>


class ScientificSpinBox : public QDoubleSpinBox {
Q_OBJECT
public:
    explicit ScientificSpinBox(QWidget* parent = nullptr);

protected:
    QValidator::State validate(QString& input, int& pos) const override;
    double valueFromText(const QString& text) const override;
    QString textFromValue(double val) const override;
    void stepBy(int steps) override;

private:
    QRegularExpressionValidator m_validator;
};


#endif //SCIENTIFICSPINBOX_H
