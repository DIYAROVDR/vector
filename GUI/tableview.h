#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>
#include <QHeaderView>

class TableView : public QTableView
{
    Q_OBJECT // Макрос Q_OBJECT для поддержки сигналов и слотов (если нужно)

public:
    explicit TableView(QWidget *parent = nullptr); // Конструктор

    // Дополнительные методы, если нужны
};

#endif // TABLEVIEW_H
