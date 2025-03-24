#ifndef DOUBLEVALIDATORDELEGATE_H
#define DOUBLEVALIDATORDELEGATE_H

#include <QItemDelegate>
#include <QLineEdit>
#include <QDoubleValidator>

class DoubleValidatorDelegate : public QItemDelegate {
    Q_OBJECT
public:
    explicit DoubleValidatorDelegate(QObject *parent = nullptr);
protected:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem& option,const QModelIndex& index) const override;
};

#endif // DOUBLEVALIDATORDELEGATE_H
