#ifndef DOUBLESPINBOXDELEGATE_H
#define DOUBLESPINBOXDELEGATE_H

#include <QStyledItemDelegate>
#include <QDoubleSpinBox>

class DoubleSpinBoxDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit DoubleSpinBoxDelegate(
        double maxval = 1E+9,
        double minval = 1E-9,
        double step = 0.1,
        int decimal = 1,
        QObject* parent = nullptr
    );
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model,const QModelIndex& index) const override;
private:
    double maxval = 1E+9;
    double minval = 1E-9;
    double step = 0.1;
    int decimal = 1;
};

#endif // DOUBLESPINBOXDELEGATE_H
