#include "gridviewwidget.h"

GridViewWidget::GridViewWidget(QWidget *parent) : QWidget{parent} {
    multiviewwidget = new MultiViewWidget(this);
    selectcubewidget = new SelectCubeWidget();
    viewsettingswidget = new ViewSettingsWidget();

    spl_1 = new QSplitter(Qt::Horizontal);
    spl_2 = new QSplitter(Qt::Vertical);

    spl_2->addWidget(selectcubewidget);
    spl_2->addWidget(viewsettingswidget);
    spl_2->setStretchFactor(0,5);
    spl_2->setStretchFactor(1,1);

    spl_1->addWidget(spl_2);
    spl_1->addWidget(multiviewwidget);
    spl_1->setStretchFactor(0,1);
    spl_1->setStretchFactor(1,5);

    spl_1->setOpaqueResize(false);
    spl_2->setOpaqueResize(false);

    QHBoxLayout* layout_1 = new QHBoxLayout();
    layout_1->setContentsMargins(0,0,0,0);
    setLayout(layout_1);
    layout()->addWidget(spl_1);

    connect(viewsettingswidget,&ViewSettingsWidget::showAxis,multiviewwidget,&MultiViewWidget::showAxis);
    connect(viewsettingswidget,&ViewSettingsWidget::showOrientationAxis,multiviewwidget,&MultiViewWidget::showOrientationAxis);
    connect(viewsettingswidget,&ViewSettingsWidget::showLegend,multiviewwidget,&MultiViewWidget::showLegend);
}

void GridViewWidget::loadGrid() {
    multiviewwidget->loadGrid();
}

void GridViewWidget::updateGrid(size_t step) {
    multiviewwidget->updateGrid(step);
}
