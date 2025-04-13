#include "mainwindow.h"
#include "homewidget.h"
#include "projectdatawidget.h"
#include "gridviewwidget.h"
#include "timesliderwidget.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), buttonGroup(new QButtonGroup(this)), currentButton(nullptr), spl(nullptr), solver(nullptr), thread(nullptr) {
    mainFont.setPixelSize(14);
    setFont(mainFont);
    setWindowIcon(QIcon(":/images/nabla.png"));
    setWindowTitle("Вектор");
    centralWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 5);

    timeSliderWidget = new TimeSliderWidget(centralWidget);
    mainLayout->addWidget(timeSliderWidget);

    mainWidgets = new QWidget(centralWidget);
    hLayout = new QHBoxLayout(mainWidgets);
    hLayout->setSpacing(0);
    hLayout->setContentsMargins(1, 1, 1, 0);

    btnWidget = new QWidget(mainWidgets);
    btnLayout = new QVBoxLayout(btnWidget);
    btnLayout->setSpacing(0);
    btnLayout->setContentsMargins(0, 0, 0, 0);

    btnHome = new QPushButton(btnWidget);
    btnHome->setIcon(QIcon(":/images/home.png"));
    btnHome->setIconSize(QSize(30, 30));
    btnHome->setCheckable(true);
    btnHome->setChecked(true);
    btnHome->setFlat(true);
    btnLayout->addWidget(btnHome);

    btnModel = new QPushButton(btnWidget);
    btnModel->setIcon(QIcon(":/images/data.png"));
    btnModel->setIconSize(QSize(30, 30));
    btnModel->setCheckable(true);
    btnModel->setFlat(true);
    btnLayout->addWidget(btnModel);

    btnGrid = new QPushButton(btnWidget);
    btnGrid->setIcon(QIcon(":/images/grid.png"));
    btnGrid->setIconSize(QSize(30, 30));
    btnGrid->setCheckable(true);
    btnGrid->setFlat(true);
    btnLayout->addWidget(btnGrid);

    btnPlot = new QPushButton(btnWidget);
    btnPlot->setIcon(QIcon(":/images/plot.png"));
    btnPlot->setIconSize(QSize(30, 30));
    btnPlot->setCheckable(true);
    btnPlot->setFlat(true);
    btnLayout->addWidget(btnPlot);

    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    btnLayout->addItem(verticalSpacer);

    hLayout->addWidget(btnWidget);

    stackedWidget = new QStackedWidget(mainWidgets);
    stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    stackedWidget->setFrameShape(QFrame::NoFrame);
    stackedWidget->setCurrentIndex(1);

    pageHome = new QWidget(stackedWidget);
    homeLayout = new QVBoxLayout(pageHome);
    homeLayout->setSpacing(0);
    homeLayout->setContentsMargins(0, 0, 0, 0);
    homeWidget = new HomeWidget(pageHome);
    homeLayout->addWidget(homeWidget);
    stackedWidget->addWidget(pageHome);

    pageModel = new QWidget(stackedWidget);
    modelLayout = new QVBoxLayout(pageModel);
    modelLayout->setSpacing(0);
    modelLayout->setContentsMargins(0, 0, 0, 0);
    modelDataWidget = new ProjectDataWidget(pageModel);
    modelLayout->addWidget(modelDataWidget);
    stackedWidget->addWidget(pageModel);

    pageGrid = new QWidget(stackedWidget);
    gridLayout = new QVBoxLayout(pageGrid);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridViewWidget = new GridViewWidget(pageGrid);
    gridLayout->addWidget(gridViewWidget);
    stackedWidget->addWidget(pageGrid);

    pagePlot = new QWidget(stackedWidget);
    stackedWidget->addWidget(pagePlot);

    hLayout->addWidget(stackedWidget);

    mainLayout->addWidget(mainWidgets);

    setCentralWidget(centralWidget);

    buttonGroup->addButton(btnHome, 0);
    buttonGroup->addButton(btnModel, 1);
    buttonGroup->addButton(btnGrid, 2);
    buttonGroup->addButton(btnPlot, 3);

    currentButton = btnHome;
    stackedWidget->setCurrentIndex(0);

    spl = new QSplitter(Qt::Vertical, centralWidget);
    spl->addWidget(mainWidgets);
    spl->setOpaqueResize(false);
    spl->setStretchFactor(0, 1);
    spl->setStretchFactor(1, 2);

    mainLayout->addWidget(spl);

    connect(homeWidget, &HomeWidget::createProject, modelDataWidget, &ProjectDataWidget::createProject);
    connect(modelDataWidget, &ProjectDataWidget::updateGrid, gridViewWidget, &GridViewWidget::loadGrid);
    connect(timeSliderWidget, &TimeSliderWidget::sp_timestep_changed, gridViewWidget, &GridViewWidget::updateGrid);
    connect(buttonGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &MainWindow::onButtonClicked);
}

MainWindow::~MainWindow() {
    delete spl;
    delete buttonGroup;
}

void MainWindow::on_btn_run_clicked() {
    static QThread* thread = nullptr;
    static Solver* solver = nullptr;

    if (solver != nullptr) {
        solver->stop();
        thread->quit();
        thread->wait();
        delete solver;
        solver = nullptr;
        delete thread;
        thread = nullptr;
        return;
    }

    solver = new Solver(
        modelDataWidget->getGrid(),
        modelDataWidget->getWell()
        );

    double dt = 0.01 * 86400;
    double T = 5 * 86400;

    timeSliderWidget->setLastStep(static_cast<size_t>(T / dt));
    solver->setup(T, dt);

    thread = new QThread();
    //solver->moveToThread(thread);

    //connect(thread, &QThread::started, solver, &Solver::run);
    //connect(solver, &Solver::finished, thread, &QThread::quit);
    //connect(solver, &Solver::finished, solver, &Solver::deleteLater);
    //connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    /*
    connect(solver, &Solver::progressUpdated, this, [=](double currentTime, double totalTime) {
        double progress = (currentTime / totalTime) * 100;
        timeSliderWidget->updateTimeStep();
    });

    connect(solver, &Solver::finished, this, [=]() {
        solver = nullptr;
        thread = nullptr;
        timeSliderWidget->stop();
    });
    */
    thread->start();
}

void MainWindow::onButtonClicked(QAbstractButton* button) {
    if (!H5FileManager::instance().isOpen()) {
        if (currentButton) {
            QMessageBox::warning(this, tr("Ошибка"), tr("Создайте или откройте проект!"), QMessageBox::Ok);
            currentButton->setChecked(true);
        }
        return;
    }

    int index = buttonGroup->id(button);

    stackedWidget->setCurrentIndex(index);

    if (currentButton) {
        currentButton->setChecked(false);
    }

    button->setChecked(true);
    currentButton = button;
}
