#include "pvtdatawidget.h"

PVTDataWidget::PVTDataWidget(QWidget* parent) :
                QWidget(parent),
                splmain(new QSplitter(Qt::Horizontal)),
                treeView(new TreeView(this)),
                charView(new ChartView(this)) {
    mainfont.setPointSize(10);
    setFont(mainfont);
    boldfont.setBold(true);
    pvtmodel = new PVTModel();


    treeView->setModel(pvtmodel);
    treeView->setItemDelegateForColumn(1, new MultiDelegate(this));
    treeView->expandAll();

    splmain->addWidget(treeView);
    splmain->addWidget(charView);

    setLayout(new QVBoxLayout(this));
    layout()->addWidget(splmain);
    //layout()->addItem(btnLayout);
    layout()->setContentsMargins(5,0,5,0);
}

void PVTDataWidget::setRegionCount(int count) {

}

void PVTDataWidget::openProject()
{

}

PVTDataWidget::~PVTDataWidget()
{

}

