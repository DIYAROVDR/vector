#include "wellsdatawidget.h"

WellsDataWidget::WellsDataWidget(QWidget* parent):
                QWidget(parent),
                h5filemanager(H5FileManager::instance()) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->setContentsMargins(5,0,0,0);

    wellstreewidget = new WellsTreeWidget();
    spl = new QSplitter(Qt::Horizontal);
    tabWidget = new QTabWidget(this);

    trajectoryView = new TableView(this);
    controlView = new TableView(this);
    perforationView = new TableView(this);
    historyView = new TableView(this);  // Добавлено для истории

    btn_add_row_welltrack = new QPushButton("Добавить", this);
    btn_del_row_welltrack = new QPushButton("Удалить", this);
    btn_apply_welltracks = new QPushButton("Применить", this);

    // Добавляем вкладки
    tabWidget->addTab(trajectoryView, "Траектории");
    tabWidget->addTab(controlView, "Контроль");
    tabWidget->addTab(perforationView, "Перфорации");
    tabWidget->addTab(historyView, "История");  // Добавлено для истории

    spl->addWidget(wellstreewidget);
    spl->addWidget(tabWidget);
    //spl->setHandleWidth(0);
    spl->setOpaqueResize(false);
    spl->setStretchFactor(0,1);
    spl->setStretchFactor(1,3);

    QSpacerItem* headerspacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(btn_add_row_welltrack);
    buttonLayout->addWidget(btn_del_row_welltrack);
    buttonLayout->addWidget(btn_apply_welltracks);
    buttonLayout->addItem(headerspacer);

    layout->addLayout(buttonLayout);
    layout->addWidget(spl);

    setLayout(layout);

    connect(btn_add_row_welltrack, &QPushButton::clicked, this, &WellsDataWidget::on_btn_add_row_welltrack_clicked);
    connect(btn_del_row_welltrack, &QPushButton::clicked, this, &WellsDataWidget::on_btn_del_row_welltrack_clicked);
    connect(btn_apply_welltracks, &QPushButton::clicked, this, &WellsDataWidget::on_btn_apply_welltracks_clicked);

    connect(wellstreewidget, &WellsTreeWidget::wellAdded, this, &WellsDataWidget::wellAdded);
    connect(wellstreewidget, &WellsTreeWidget::wellDeleted, this, &WellsDataWidget::wellDeleted);
    connect(wellstreewidget, &WellsTreeWidget::wellItemClicked, this, &WellsDataWidget::changedCurrentWell);
    connect(wellstreewidget, &WellsTreeWidget::wellNameChanged, this, &WellsDataWidget::wellNameChanged);
}

WellsDataWidget::~WellsDataWidget() {
    // Деструктор
}

void WellsDataWidget::on_btn_del_row_welltrack_clicked() {
    QModelIndex currentIndex;
    int tab = tabWidget->currentIndex();

    switch (tab) {
    case 0:  // WellTrajectory
        currentIndex = trajectoryView->currentIndex();
        if (currentIndex.isValid()) {
            trajectoryModel->removeRow(currentIndex.row());
        }
        break;
    case 1:  // WellControl
        currentIndex = controlView->currentIndex();
        if (currentIndex.isValid()) {
            controlModel->removeRow(currentIndex.row());
        }
        break;
    case 2:  // Perforation
        currentIndex = perforationView->currentIndex();
        if (currentIndex.isValid()) {
            perforationModel->removeRow(currentIndex.row());
        }
        break;
    case 3:  // History
        // Логика для удаления строки из истории
        break;
    }
}

void WellsDataWidget::on_btn_add_row_welltrack_clicked() {
    int tab = tabWidget->currentIndex();

    switch (tab) {
    case 0:  // WellTrajectory
        trajectoryModel->insertRow(trajectoryModel->rowCount());
        break;
    case 1:  // WellControl
        controlModel->insertRow(controlModel->rowCount());
        break;
    case 2:  // Perforation
        perforationModel->insertRow(perforationModel->rowCount());
        break;
    case 3:  // History
        // Логика для добавления строки в историю
        break;
    }
}

void WellsDataWidget::on_btn_apply_welltracks_clicked() {
    // Реализация функции
}

void WellsDataWidget::wellAdded(const QString& wellname) {
    currentWellName = wellname;
    trajectoryModel->setCurrentWell(wellname);
    perforationModel->setCurrentWell(wellname);
    controlModel->setCurrentWell(wellname);
}

void WellsDataWidget::wellDeleted(const QString& wellname) {
    // Реализация функции
}

void WellsDataWidget::changedCurrentWell(const QString& wellname) {
    currentWellName = wellname;
    trajectoryModel->setCurrentWell(wellname);
    perforationModel->setCurrentWell(wellname);
    controlModel->setCurrentWell(wellname);
}

void WellsDataWidget::wellNameChanged(const QString& oldname, const QString& newname) {
    // Реализация функции
}

void WellsDataWidget::openProject(PhysicalQuantity* physicalquantity) {
    trajectoryModel = new WellTrajectoryModel(this);
    perforationModel = new PerforationModel(this);
    controlModel = new WellControlModel(this, physicalquantity);

    controlView->setItemDelegateForColumn(0, new DateEditDelegate());
    controlView->setItemDelegateForColumn(1, new ComboBoxDelegate(controlModel->controlItems()));
    controlView->setItemDelegateForColumn(2, new DoubleSpinBoxDelegate());

    trajectoryView->setItemDelegateForColumn(1, new DoubleSpinBoxDelegate());
    trajectoryView->setItemDelegateForColumn(0, new DoubleSpinBoxDelegate());
    trajectoryView->setItemDelegateForColumn(1, new DoubleSpinBoxDelegate());

    perforationView->setItemDelegateForColumn(0, new DateEditDelegate());
    perforationView->setItemDelegateForColumn(1, new DateEditDelegate());
    perforationView->setItemDelegateForColumn(2, new DoubleSpinBoxDelegate());
    perforationView->setItemDelegateForColumn(3, new DoubleSpinBoxDelegate());
    perforationView->setItemDelegateForColumn(3, new DoubleSpinBoxDelegate());
    perforationView->setItemDelegateForColumn(3, new DoubleSpinBoxDelegate());
    perforationView->setItemDelegateForColumn(3, new DoubleSpinBoxDelegate());

    trajectoryView->setModel(trajectoryModel);
    perforationView->setModel(perforationModel);
    controlView->setModel(controlModel);
}
