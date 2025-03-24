#include "projectdatawidget.h"

ProjectDataWidget::ProjectDataWidget(QWidget* parent) :
                   QWidget(parent),
                   layout(new QHBoxLayout(this)),
                   treeWidget(new QTreeWidget(this)),
                   stackedWidget(new QStackedWidget(this)),
                   spl(new QSplitter(Qt::Horizontal)),
                   physicalquantity(new PhysicalQuantity()),
                   genpropwidget(new GenPropWidget(this)),
                   griddatawidget(new GridDataWidget(this)),
                   pvtdatawidget(new PVTDataWidget(this)),
                   rockdatawidget(new RockDataWidget(this)),
                   initdatawidget(new InitDataWidget(this)),
                   fillcubewidget(new FillCubeWidget(this, physicalquantity)),
                   wellsdatawidget(new WellsDataWidget(this)),
                   h5filemanager(H5FileManager::instance()) {

    mainFont.setPointSize(10);
    boldFont.setBold(true);

    initTreeWidget();

    spl->addWidget(treeWidget);
    spl->addWidget(stackedWidget);
    spl->setHandleWidth(0);
    spl->setOpaqueResize(false);
    spl->setStretchFactor(0,1);
    spl->setStretchFactor(1,2);

    stackedWidget->addWidget(genpropwidget);
    stackedWidget->addWidget(pvtdatawidget);
    stackedWidget->addWidget(rockdatawidget);
    stackedWidget->addWidget(griddatawidget);
    stackedWidget->addWidget(initdatawidget);
    stackedWidget->addWidget(fillcubewidget);
    stackedWidget->addWidget(wellsdatawidget);

    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
    layout->addWidget(spl);

    connect(griddatawidget,&GridDataWidget::dimensChanged,this,[&](){fillcubewidget->dimensChanged();});
}


void ProjectDataWidget::initialization() {
    emit updateGrid();
}


Grid* ProjectDataWidget::getGrid() {
    return griddatawidget->getGrid();
}


Well* ProjectDataWidget::getWell() {
    return griddatawidget->getWell();
}


void ProjectDataWidget::createProject(const QString& dir) {
    hdf5FilePath = dir;
    projectPath = dir;

    fileName = dir.split(QChar('/')).last();
    projectPath.chop(fileName.length());

    h5filemanager.openFile(hdf5FilePath.toStdString());
    fillcubewidget->setProjectPath(projectPath);

    genpropwidget->openProject(physicalquantity);
    griddatawidget->openProject();
    pvtdatawidget->openProject();
    rockdatawidget->openProject();
    fillcubewidget->openProject();
    initdatawidget->openProject();
    wellsdatawidget->openProject(physicalquantity);

    emit openProject();
}


void ProjectDataWidget::updateData() {

}


void ProjectDataWidget::handleItemClicked(QTreeWidgetItem* item) {    
    if (item == genPropItem) {
        stackedWidget->setCurrentIndex(0); // Виджет общих данных модели
    }
    else if (item == pvtItem ) {
        stackedWidget->setCurrentIndex(1); // Виджет данных PVT модели
    }
    else if (item == rockItem) {
        stackedWidget->setCurrentIndex(2); // Виджет данных породы
    }
    else if (item == gridItem ) {
        stackedWidget->setCurrentIndex(3); // Виджет геометрия модели
    }
    else if (item == initStateItem) {
        stackedWidget->setCurrentIndex(4); // Виджет инициализации модели
    }
    else if (item == cubesItem) {
        stackedWidget->setCurrentIndex(5); // Виджет данных кубов
    }
    else if (item == wellsDataItem) {
        stackedWidget->setCurrentIndex(6); // Виджет данных скважин
    }
}


void ProjectDataWidget::openProjectDir() {
    if (QDir::setCurrent(projectPath)) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(projectPath));
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть директорию: " + projectPath);
    }
}


void ProjectDataWidget::initTreeWidget() {
    connect(treeWidget, &QTreeWidget::itemClicked, this, &ProjectDataWidget::handleItemClicked);
    treeWidget->setColumnCount(1);
    treeWidget->setFont(mainFont);

    treeWidget->header()->hide();
    headerWidgetItem = new QTreeWidgetItem(treeWidget);
    treeWidget->setItemWidget(headerWidgetItem,0,createHeaderWidget());

    treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
    treeWidget->setColumnCount(1);
    treeWidget->setItemDelegate(new ttDelegate(treeWidget));

    modelPropItem = new QTreeWidgetItem(treeWidget);
    modelPropItem->setText(0, "Свойства модели");
    modelPropItem->setFont(0, boldFont);

    genPropItem = new QTreeWidgetItem(modelPropItem);
    genPropItem->setText(0, "Общее");

    gridItem = new QTreeWidgetItem(modelPropItem);
    gridItem->setText(0, "Расчетная сетка");

    pvtItem = new QTreeWidgetItem(modelPropItem);
    pvtItem->setText(0, "Уравнения состояния");

    rpcItem = new QTreeWidgetItem(modelPropItem);
    rpcItem->setText(0, "Относительные фазовые проницаемости");

    rockItem = new QTreeWidgetItem(modelPropItem);
    rockItem->setText(0, "Свойства горной породы");

    wellsDataItem = new QTreeWidgetItem(modelPropItem);
    wellsDataItem->setText(0, "Скважины");

    cubesItem = new QTreeWidgetItem(modelPropItem);
    cubesItem->setText(0, "Кубы");

    initStateItem = new QTreeWidgetItem(modelPropItem);
    initStateItem->setText(0, "Инициализация");

    modelPropItem->setBackground(0, QColor(173, 216, 230));
    treeWidget->expandAll();
}


QWidget* ProjectDataWidget::createHeaderWidget() {
    QWidget* headerWidget = new QWidget(this);

    QHBoxLayout* layout = new QHBoxLayout(headerWidget);

    QLineEdit* searchLineEdit = new QLineEdit(headerWidget);
    searchLineEdit->setPlaceholderText("Быстрый поиск");
    searchLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QPushButton* btn_open = new QPushButton(QIcon(":/images/folder_file.png"), "", headerWidget);
    QPushButton* btn_update = new QPushButton(QIcon(":/images/update.png"), "", headerWidget);
    QPushButton* btn_init = new QPushButton(QIcon(":/images/initialization.png"), "", headerWidget);

    btn_open->setFlat(true);
    btn_update->setFlat(true);
    btn_init->setFlat(true);

    btn_open->setIconSize(QSize(20,20));
    btn_update->setIconSize(QSize(20,20));
    btn_init->setIconSize(QSize(20,20));


    connect(btn_open, &QPushButton::clicked, this, &ProjectDataWidget::openProjectDir);
    connect(btn_update,&QPushButton::clicked,this,&ProjectDataWidget::updateData);
    connect(btn_init,&QPushButton::clicked,this,&ProjectDataWidget::initialization);

    layout->addWidget(searchLineEdit);
    layout->addWidget(btn_update);
    layout->addWidget(btn_init);
    layout->addWidget(btn_open);
    layout->setContentsMargins(5, 5, 5, 5);

    return headerWidget;
}

