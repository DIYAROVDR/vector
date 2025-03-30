#include "genpropwidget.h"

GenPropWidget::GenPropWidget(QWidget* parent) :
                QWidget(parent),
                treeView(new TreeView(this)),
                btnLayout(new QHBoxLayout()),
                btnApply(new QPushButton("Применить",this)) {

    mainFont.setPointSize(10);
    boldFont.setBold(true);
    btnLayout->addWidget(btnApply);
    setLayout(new QVBoxLayout(this));
    layout()->addWidget(treeView);
    layout()->addItem(btnLayout);
    layout()->setContentsMargins(5,0,5,0);

    connect(btnApply, &QPushButton::clicked, this, &GenPropWidget::on_btn_apply_clicked);
}

GenPropWidget::~GenPropWidget() {

}

void GenPropWidget::openProject(PhysicalQuantity* physicalQuantity) {
    model = new GeneralModel(this, physicalQuantity);
    treeView->setModel(model);
    treeView->setItemDelegateForColumn(1, new MultiDelegate());
    treeView->expandAll();
}

void GenPropWidget::on_btn_apply_clicked() {
    model->saveData();
}
