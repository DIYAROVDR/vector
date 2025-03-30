#include "pvtdatawidget.h"

PVTDataWidget::PVTDataWidget(QWidget* parent, PhysicalQuantity* physicalquantity) :
        QWidget(parent),
        splmain(new QSplitter(Qt::Horizontal)),
        spldata(new QSplitter(Qt::Vertical)),
        treeView(new TreeView(this)),
        tableView(new TableView(this)),
        charView(new ChartView(this)),
        pvtTreeModel(new PVTTreeModel(physicalquantity)) {

    pvtTableModel = new PVTTableModel();
    pvtTableModel->setTreeModel(pvtTreeModel);
    tableView->setModel(pvtTableModel);

    treeView->setModel(pvtTreeModel);
    treeView->setItemDelegateForColumn(1, new MultiDelegate(this));
    treeView->expandAll();
    spldata->addWidget(treeView);
    spldata->addWidget(tableView);

    splmain->addWidget(spldata);
    splmain->addWidget(charView);
    splmain->setHandleWidth(0);
    splmain->setOpaqueResize(false);
    splmain->setStretchFactor(0,1);
    splmain->setStretchFactor(1,4);

    charView->setChartTitle("Данные");

    setLayout(new QVBoxLayout(this));
    layout()->addWidget(splmain);
    //layout()->addItem(btnLayout);
    layout()->setContentsMargins(5,0,5,0);

    connect(treeView, &QTreeView::clicked, this, &PVTDataWidget::onTreeViewItemClicked);
}

void PVTDataWidget::openProject() {

}

PVTDataWidget::~PVTDataWidget() {

}

void PVTDataWidget::onTreeViewItemClicked(const QModelIndex& index) {
    pvtTreeModel->setData(index, true , Qt::ForegroundRole);
    pvtTreeModel->setData(index, QVariant(), Qt::UserRole + 5);

    //QVariant value = pvtTreeModel->data(index, Qt::UserRole + 3);
}

