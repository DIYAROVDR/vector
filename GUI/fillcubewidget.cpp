#include "fillcubewidget.h"

FillCubeWidget::FillCubeWidget(QWidget* parent, PhysicalQuantity* physicalquantity) :
    QWidget(parent),
    physicalquantity(physicalquantity),
    h5filemanager(H5FileManager::instance()),
    cubestreemodel(new CubesTreeModel(this, physicalquantity)),
    cubetreewidget(new CubeTreeWidget(this)),
    layerfillcubemodel(new LayerFillCubeModel(this)) {

    cubetreewidget->setModel(cubestreemodel);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    splmain = new QSplitter(Qt::Horizontal);
    splmain->addWidget(cubetreewidget);

    cmb_type_edit_tab = new QComboBox();
    btn_apply_table_data = new QPushButton("Применить");
    btn_clear_tabe = new QPushButton("Очистить");
    cmb_type_edit_tab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    cmb_type_edit_tab->addItems({
        "По слоям",
        "По интервалам",
        "Для каждой ячейки"
    });

    QHBoxLayout* tableButtonLayout = new QHBoxLayout();
    tableButtonLayout->addWidget(cmb_type_edit_tab);
    tableButtonLayout->addWidget(btn_apply_table_data);
    tableButtonLayout->addWidget(btn_clear_tabe);

    tabWidget = new QTabWidget(this);
    QWidget* tabInput = new QWidget();
    QVBoxLayout* tabInputLayout = new QVBoxLayout(tabInput);
    tableView = new TableView();
    tableView->setModel(layerfillcubemodel);
    tableView->setItemDelegate(new MultiDelegate());
    tabInputLayout->addLayout(tableButtonLayout);
    tabInputLayout->addWidget(tableView);
    tabWidget->addTab(tabInput, "Табличный ввод");
    tabWidget->setTabIcon(0, QIcon(":/images/table.png"));
    splmain->addWidget(tabWidget);


    QWidget* tabScript = new QWidget();
    QVBoxLayout* tabScriptLayout = new QVBoxLayout(tabScript);
    cmb_scripts_lists = new QComboBox();
    btn_add_script = new QPushButton("Добавить");
    btn_delete_script = new QPushButton("Удалить");
    btn_load_script = new QPushButton("Загрузить");
    btn_clear_code = new QPushButton("Очистить");
    btn_run_script = new QPushButton("Запустить скрипт");

    cmb_scripts_lists->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(cmb_scripts_lists);
    buttonLayout->addWidget(btn_add_script);
    buttonLayout->addWidget(btn_delete_script);
    buttonLayout->addWidget(btn_load_script);
    buttonLayout->addWidget(btn_clear_code);
    buttonLayout->addWidget(btn_run_script);

    tabScriptLayout->addLayout(buttonLayout);

    spl = new QSplitter(Qt::Vertical);
    plainTextEdit_result = new QPlainTextEdit();
    plainTextEdit_result->setPlaceholderText("Здесь будет выводиться результат работы Python скрипта");
    plainTextEdit_result->setReadOnly(true);

    codeEditor = new CodeEditor();

    spl->addWidget(codeEditor);
    spl->addWidget(plainTextEdit_result);
    spl->setHandleWidth(3);
    spl->setOpaqueResize(false);
    spl->setStretchFactor(0, 2);
    spl->setStretchFactor(1, 1);

    tabScriptLayout->addWidget(spl);
    tabWidget->addTab(tabScript, "Python скрипт");
    tabWidget->setTabIcon(1, QIcon(":/images/python.png"));

    mainLayout->addWidget(splmain);
    mainLayout->setContentsMargins(5, 0, 5, 0);

    pythonModule = new PythonModule();
    pythonModule->setOutput(plainTextEdit_result);

    QRegularExpression regex("^[a-zA-Z_][a-zA-Z0-9_]*$");
    QValidator* validator = new QRegularExpressionValidator(regex, this);
    cmb_scripts_lists->setEditable(true);
    cmb_scripts_lists->lineEdit()->setValidator(validator);
    cmb_scripts_lists->setEnabled(false);

    connect(
        cubetreewidget,
        &CubeTreeWidget::cubeItemClicked,
        this,[&](const QString& name){layerfillcubemodel->setCurrentCube(name);}
    );

    connect(btn_apply_table_data, &QPushButton::clicked, this, &FillCubeWidget::on_btn_apply_clicked);

    connect(btn_run_script, &QPushButton::clicked, this, &FillCubeWidget::on_btn_run_script_clicked);
    connect(btn_clear_code, &QPushButton::clicked, this, &FillCubeWidget::on_btn_clear_code_clicked);
    connect(btn_add_script, &QPushButton::clicked, this, &FillCubeWidget::on_btn_add_script_clicked);
    connect(btn_delete_script, &QPushButton::clicked, this, &FillCubeWidget::on_btn_delete_script_clicked);
}

FillCubeWidget::~FillCubeWidget() {
    delete pythonModule;
}

void FillCubeWidget::setProjectPath(const QString& path) {
    if (pythonModule) {
        pythonModule->setOutputDirectory(path);
    }
}

void FillCubeWidget::openProject() {
    // Реализация открытия проекта
}

void FillCubeWidget::dimensChanged() {
    layerfillcubemodel->updateDimens();
}

void FillCubeWidget::on_btn_run_script_clicked() {
    QString scriptCode = codeEditor->toPlainText();
    if (scriptCode.trimmed().isEmpty()) {
        QMessageBox::warning(
        this,
        tr("Ошибка"),
        tr("Отсутствует тело Python скрипта. Пожалуйста, введите код в редакторе."),
    QMessageBox::Ok
        );
        return;
    }

    plainTextEdit_result->clear();
    pythonModule->run(scriptCode);
}

void FillCubeWidget::on_btn_clear_code_clicked() {
    codeEditor->clear();
    plainTextEdit_result->clear();
}

void FillCubeWidget::on_btn_add_script_clicked() {
    QString scriptCode = codeEditor->toPlainText();
    if (scriptCode.isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Код скрипта не может быть пустым."));
        return;
    }

    int scriptCount = scripts.size();
    QString scriptName = QString("script%1").arg(scriptCount + 1);

    while (scripts.contains(scriptName)) {
        scriptCount++;
        scriptName = QString("script%1").arg(scriptCount + 1);
    }

    scripts.insert(scriptName, scriptCode);
    cmb_scripts_lists->addItem(scriptName);

    if (cmb_scripts_lists->count() > 0) {
        cmb_scripts_lists->setEnabled(true);
    }
}

void FillCubeWidget::on_btn_delete_script_clicked() {
    QString scriptName = cmb_scripts_lists->currentText();
    if (scriptName.isEmpty() || !scripts.contains(scriptName)) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Скрипт для удаления не выбран или не существует."));
        return;
    }

    scripts.remove(scriptName);
    int index = cmb_scripts_lists->currentIndex();
    if (index != -1) {
        cmb_scripts_lists->removeItem(index);
    }

    if (codeEditor->toPlainText() == scriptName) {
        codeEditor->clear();
    }

    if (cmb_scripts_lists->count() == 0) {
        cmb_scripts_lists->setEnabled(false);
    }
}

void FillCubeWidget::on_btn_apply_clicked() {
    layerfillcubemodel->saveCube();
}
