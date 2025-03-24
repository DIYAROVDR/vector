#ifndef MULTIDELEGATE_H
#define MULTIDELEGATE_H

#include <QApplication>
#include <QComboBox>
#include <QStyledItemDelegate>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QPainter>
#include <QItemDelegate>

#include "delegatetype.h" // Подключаем DelegateType

class MultiDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit MultiDelegate(QObject* parent = nullptr);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // MULTIDELEGATE_H
