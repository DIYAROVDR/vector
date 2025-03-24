#include "multiviewidget.h"

MultiViewWidget::MultiViewWidget(QWidget *parent) : QTabWidget(parent) {
    mainfont.setPointSize(10);
    setFont(mainfont);
    addwgt = new AddViewWidget();
    addwgt->setFont(mainfont);
    insertTab(count() - 1, addwgt->createSpatialViewWidget(), "Вид #1");
    setCurrentIndex(count() - 2);

    addTab(addwgt, "+");

    this->setStyleSheet(
        "QTabWidget::pane {"
        "  border: 2px solid rgb(0, 0, 255);"  // Синий контур
        "  padding: 1px;"  // Отступ между вкладками и содержимым
        "}"

        "QTabBar::tab {"
        "  background: rgb(240, 240, 240);"  // Светлый серый фон для всех вкладок
        "  border: 1px solid rgb(150, 150, 150);"  // Светло-серая рамка вокруг вкладок
        "  padding: 5px 10px;"  // Отступы внутри вкладки
        "  border-radius: 2px;"  // Скругленные углы вкладок
        "  margin-right: 1px;"  // Отступ между вкладками
        "}"

        "QTabBar::tab:selected {"
        "  background-color: rgb(200, 200, 200);"  // Светло-серый фон для активной вкладки
        "}"

        "QTabBar::tab:hover {"
        "  background-color: rgb(220, 220, 220);"  // Ещё светлее серый при наведении
        "}"
    );

    connect(addwgt, &AddViewWidget::createSpatialViewWidgetBtnClicked, this, [&](SpatialViewWidget* wgt) {
        int newTabIndex = insertTab(count() - 1, wgt, QString("Вид #%1").arg(count()));
        setCurrentIndex(newTabIndex);
        addCloseButton(newTabIndex);
    });

    addCloseButton(0);
}



void MultiViewWidget::showAxis(bool flag) {
    for(int tab = 0; tab < count();++tab) {
        SpatialViewWidget* wgt = qobject_cast<SpatialViewWidget*>(widget(tab));
        if(wgt){
            wgt->showAxis(flag);
        }
    }
}


void MultiViewWidget::showOrientationAxis(bool flag) {
    for(int tab = 0; tab < count();++tab) {
        SpatialViewWidget* wgt = qobject_cast<SpatialViewWidget*>(widget(tab));
        if(wgt){
            wgt->showOrientationAxis(flag);
        }
    }
}


void MultiViewWidget::showLegend(bool flag) {
    for(int tab = 0; tab < count();++tab) {
        SpatialViewWidget* wgt = qobject_cast<SpatialViewWidget*>(widget(tab));
        if(wgt){
            wgt->showLegend(flag);
        }
    }
}


void MultiViewWidget::loadGrid() {
    for(int tab = 0; tab < count();++tab) {
        SpatialViewWidget* wgt = qobject_cast<SpatialViewWidget*>(widget(tab));
        if(wgt) {
            H5FileManager& file = H5FileManager::instance();
            wgt->createGrid(file.loadGrid(),file.loadScalar());
        }
    }
}


void MultiViewWidget::updateGrid(size_t step) {
    for(int tab = 0; tab < count();++tab) {
        SpatialViewWidget* wgt = qobject_cast<SpatialViewWidget*>(widget(tab));
        if(wgt) {
            H5FileManager& file = H5FileManager::instance();
            wgt->setScalar(file.loadScalar(step));
        }
    }
}

void MultiViewWidget::addCloseButton(int index) {
    QWidget* tabWidget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(tabWidget);

    QLabel* label = new QLabel(this);

    QPushButton* closeButton = new QPushButton(this);
    closeButton->setFlat(true);
    closeButton->setFixedSize(16, 16);
    closeButton->setIcon(QIcon(":/images/close.png"));

    layout->addWidget(label);
    layout->addWidget(closeButton);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);

    this->tabBar()->setTabButton(index, QTabBar::RightSide, tabWidget);
    connect(closeButton, &QPushButton::clicked, this, [=]() {
        removeTab(index);
    });
}

