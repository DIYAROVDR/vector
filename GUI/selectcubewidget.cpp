#include "selectcubewidget.h"

SelectCubeWidget::SelectCubeWidget(QWidget* parent): QTreeWidget(parent) {
    boldFont.setBold(true);
    mainFont.setPointSize(10);


    initTreeWidget();
    // Подключение слота для обработки двойного клика по элементу
    // connect(this, &QTreeWidget::itemDoubleClicked, this, &SelectCubeWidget::handleItemDoubleClicked);


}

void SelectCubeWidget::addInitialCubeItem(const QString& name) {
    QTreeWidgetItem* item = new QTreeWidgetItem(initialCubeItem);
    item->setText(0, name);
    initialCubeItem->addChild(item);
    itemToCubeNameMap[item] = name; // Добавляем соответствие в карту
}

void SelectCubeWidget::addCalculatedCubeItem(const QString& name) {
    QTreeWidgetItem* item = new QTreeWidgetItem(calculatedCubeItem);
    item->setText(0, name);
    calculatedCubeItem->addChild(item);
    itemToCubeNameMap[item] = name; // Добавляем соответствие в карту
}

void SelectCubeWidget::addUserCubeItem(const QString& name) {
    QTreeWidgetItem* item = new QTreeWidgetItem(userCubesItem);
    item->setText(0, name);
    userCubesItem->addChild(item);
    itemToCubeNameMap[item] = name; // Добавляем соответствие в карту
}

void SelectCubeWidget::initTreeWidget() {
    setFont(mainFont);
    setColumnCount(1);
    setHeaderHidden(true);

    setItemDelegate(new ttDelegate(this));
    setSelectionMode(QAbstractItemView::NoSelection);

    initialCubeItem = new QTreeWidgetItem(this);
    initialCubeItem->setText(0, "Статические кубы");
    initialCubeItem->setBackground(0, QColor(173, 216, 230));
    initialCubeItem->setFont(0,boldFont);

    calculatedCubeItem = new QTreeWidgetItem(this);
    calculatedCubeItem->setText(0, "Динамические кубы");
    calculatedCubeItem->setBackground(0, QColor(173, 216, 230));
    calculatedCubeItem->setFont(0,boldFont);

    userCubesItem = new QTreeWidgetItem(this);
    userCubesItem->setText(0, "Пользовательские кубы");
    userCubesItem->setBackground(0, QColor(173, 216, 230));
    userCubesItem->setFont(0,boldFont);

    expandAll();
}

void SelectCubeWidget::handleItemDoubleClicked(QTreeWidgetItem* item) {
    if (itemToCubeNameMap.find(item) != itemToCubeNameMap.end()) {
        QString cubeName = itemToCubeNameMap[item];
        emit cubeNameDoubleClicked(cubeName);
    }
}

