//
// Created by Damir on 27.03.2025.
//

#ifndef VECTOR_SCIENTIFICSPINBOX_H
#define VECTOR_SCIENTIFICSPINBOX_H

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


#endif //VECTOR_SCIENTIFICSPINBOX_H
