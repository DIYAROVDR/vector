#include "homewidget.h"

HomeWidget::HomeWidget(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* headerLayout = new QHBoxLayout();

    // Создаем кнопки
    btn_create_new_project = new QPushButton("Создать проект", this);
    btn_open_project = new QPushButton("Открыть проект", this);

    QSpacerItem* headerspacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    // Настройка стилей для кнопок
    btn_create_new_project->setStyleSheet(
        "QPushButton {"
        "  background-color: rgb(35, 178, 106);"
        "  color: white;"
        "  border-radius: 5px;"
        "  border: none;"
        "  padding: 10px 20px;"
        "}"
        "QPushButton:hover {"
        "  background-color: rgb(70, 220, 138);"
        "}"
        );

    btn_open_project->setStyleSheet(
        "QPushButton {"
        "  border-radius: 5px;"
        "  border: 2px solid black;"
        "  padding: 10px 20px;"
        "}"
        "QPushButton:hover {"
        "  border: 3px solid black;"
        "}"
        );

    // Создаем лейблы
    label = new AspectRatioPixmapLabel(this);
    label->setPixmap(QPixmap(":/images/vector_logo_zero.png"));
    label->setMaximumHeight(100);
    label->setMaximumWidth(600);
    label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    lbl_project = new QLabel("Проекты", this);
    lbl_project->setStyleSheet("color: rgb(108, 108, 108);");
    QFont fontlabel;
    fontlabel.setPointSize(14);
    lbl_project->setFont(fontlabel);

    // Создаем область прокрутки
    area = new QScrollArea(this);
    scrollAreaWidgetContents = new QWidget();
    QVBoxLayout* scrollLayout = new QVBoxLayout(scrollAreaWidgetContents);
    scrollLayout->setAlignment(Qt::AlignTop);
    area->setWidget(scrollAreaWidgetContents);
    area->setWidgetResizable(true);
    //area->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    // Добавляем элементы в основной макет

    headerLayout->addWidget(label);
    headerLayout->addItem(headerspacer);
    headerLayout->addWidget(btn_create_new_project);
    headerLayout->addWidget(btn_open_project);
    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(lbl_project);
    mainLayout->addWidget(area);

    setLayout(mainLayout);

    // Подключаем слоты и сигналы
    connect(btn_create_new_project, &QPushButton::clicked, this, &HomeWidget::on_btn_create_new_project_clicked);
}

HomeWidget::~HomeWidget() {
    // Деструктор
}

void HomeWidget::on_btn_create_new_project_clicked() {
    // Устанавливаем начальное имя файла
    QString baseName = "project";
    QString dirPath = "c:/Users/Damir/Desktop/main/pro/vector/save/";
    QString filePath = dirPath + baseName + ".h5";
    int index = 1;

    // Проверяем наличие файла и добавляем индекс, если такой файл уже существует
    while (QFile::exists(filePath)) {
        filePath = dirPath + baseName + "_" + QString::number(index++) + ".h5";
    }

    // Открываем диалог для выбора имени проекта с уникальным именем по умолчанию
    QString fileName = QFileDialog::getSaveFileName(this, "Создание нового проекта", filePath, "HDF5 файлы (*.h5)");

    if (!fileName.isEmpty()) {
        // Удостоверяемся, что файл имеет расширение .h5
        if (!fileName.endsWith(".h5", Qt::CaseInsensitive)) {
            fileName += ".h5";
        }

        // Извлекаем имя проекта и путь к директории
        projectName = QFileInfo(fileName).baseName();
        projectDir = QFileInfo(fileName).absolutePath();

        // Создаем кнопку с названием проекта и абсолютным путем к файлу
        QPushButton* newProjectButton = createButton(projectDir, projectName);

        // Добавляем кнопку в область прокрутки
        scrollAreaWidgetContents->layout()->addWidget(newProjectButton);

        // Передаем полный путь к файлу h5
        emit createProject(fileName);
    }
}

QPushButton* HomeWidget::createButton(const QString& dir, const QString& projectName) {
    QString buttonText = QString("<b>%1</b><br><small>%2</small>").arg(projectName).arg(dir);

    QPushButton* newProjectButton = new QPushButton(this);

    newProjectButton->setMinimumHeight(60); // Высота кнопки
    newProjectButton->setMinimumWidth(220); // Ширина кнопки

    QLabel* label = new QLabel(buttonText, newProjectButton);

    label->setTextFormat(Qt::RichText);
    label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    label->setStyleSheet(
        "QLabel {"
        "  font-size: 16px;"
        "}"
        "QLabel small {"
        "  font-size: 14px;"
        "}"
        );

    QHBoxLayout* layout = new QHBoxLayout(newProjectButton);
    layout->setContentsMargins(5, 5, 5, 5); // Устанавливаем отступы
    layout->setSpacing(10); // Отступ между иконкой и текстом

    QLabel* iconLabel = new QLabel(newProjectButton);
    iconLabel->setPixmap(QIcon(":/images/project.png").pixmap(32, 32));
    layout->addWidget(iconLabel);

    layout->addWidget(label);

    newProjectButton->setStyleSheet(
        "QPushButton {"
        "  border-radius: 5px;"           // Скругленные углы
        "  background-color: #f0f0f0;"    // Цвет фона
        "  border: 1px solid #dcdcdc;"    // Цвет границы
        "}"
        "QPushButton:hover {"
        "  background-color: #e0e0e0;"    // Изменение фона при наведении
        "}"
        );

    newProjectButton->setCheckable(false);

    connect(newProjectButton, &QPushButton::clicked, this, [&]() {});

    return newProjectButton;
}
