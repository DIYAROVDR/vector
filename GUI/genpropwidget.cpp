#include "genpropwidget.h"

GenPropWidget::GenPropWidget(QWidget *parent) : QWidget(parent)  {
    mainFont.setPointSize(10);
    boldFont.setBold(true);
    setLayout(new QHBoxLayout(this));
    layout()->setContentsMargins(5,0,5,0);
    treeView = new TreeView(this);
    layout()->addWidget(treeView);
}

GenPropWidget::~GenPropWidget() {

}

void GenPropWidget::openProject(PhysicalQuantity* physicalQuantity) {
    treeView->setModel(new GeneralModel(this, physicalQuantity));
    treeView->setItemDelegateForColumn(1, new MultiDelegate());
    treeView->expandAll();
}
