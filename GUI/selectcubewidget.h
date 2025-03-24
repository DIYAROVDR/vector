#ifndef DATAITEMSWIDGET_H
#define DATAITEMSWIDGET_H

#include <QTreeWidget>
#include <QDebug>
#include <QFont>
#include <map>

#include "ttdelegate.h"

class SelectCubeWidget : public QTreeWidget {
    Q_OBJECT

public:
    explicit SelectCubeWidget(QWidget* parent = nullptr);

signals:
    void cubeNameDoubleClicked(const QString& cubeName); // Сигнал для передачи названия куба

private slots:
    void handleItemDoubleClicked(QTreeWidgetItem* item); // Слот для обработки двойного клика

private:
    QFont mainFont;
    QFont boldFont;
    QTreeWidgetItem* initialCubeItem;  // Ветка для начальных свойств
    QTreeWidgetItem* calculatedCubeItem;  // Ветка для рассчитанных свойств
    QTreeWidgetItem* userCubesItem;  // Ветка для пользовательских кубов

    std::map<QTreeWidgetItem*, QString> itemToCubeNameMap; // Карта для хранения соответствий QTreeWidgetItem и названия куба

    void addInitialCubeItem(const QString& name);
    void addCalculatedCubeItem(const QString& name);
    void addUserCubeItem(const QString& name);

    void createInitialItems();
    void createCalculatedItems();
    void createUserItems();

    void initTreeWidget();
};

#endif // DATAITEMSWIDGET_H
