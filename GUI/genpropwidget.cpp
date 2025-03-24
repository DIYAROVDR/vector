#include "genpropwidget.h"

GenPropWidget::GenPropWidget(QWidget *parent) : QWidget(parent)  {
    mainFont.setPointSize(10);
    boldFont.setBold(true);
    setLayout(new QHBoxLayout(this));
    layout()->setContentsMargins(5,0,5,0);
    treeView = new QTreeView(this);
    layout()->addWidget(treeView);
    treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    treeView->expandAll();
}

GenPropWidget::~GenPropWidget() {

}

void GenPropWidget::openProject(PhysicalQuantity* physicalQuantity) {
    treeView->setModel(new GeneralModel(this, physicalQuantity));
    treeView->setItemDelegateForColumn(1, new MultiDelegate());
}
