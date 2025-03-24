#include "addviewwidget.h"

AddViewWidget::AddViewWidget(QWidget* parent) : QWidget(parent) {
    mainLayout = new QVBoxLayout(this);
    buttonLayout = new QGridLayout();

    maintFont.setPixelSize(12);
    boldFont.setPixelSize(12);
    boldFont.setBold(true);

    // Создание спейсеров
    spacer_top = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    spacer_bottom = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    spacer_left = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    spacer_right = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    // Добавление верхнего спейсера
    mainLayout->addItem(spacer_top);

    // Создание и добавление метки
    label_main = new QLabel("Выберите тип визуализации:", this);
    mainLayout->addWidget(label_main, 0, Qt::AlignCenter);
    label_main->setFont(boldFont);

    // Создание кнопок
    btn_create_spatial_view_part = new QPushButton("Пространственная (часть)", this);
    btn_create_spatial_view_4part = new QPushButton("Пространственная (4 части)", this);
    btn_create_histogram_view = new QPushButton("Гистограмма", this);
    btn_create_plot_view = new QPushButton("Графическая", this);
    btn_create_table_view = new QPushButton("Табличная", this);
    btn_create_well_profile_view = new QPushButton("Профиль скважин", this);
    btn_crate_user_view = new QPushButton("Пользовательская", this);

    btn_create_spatial_view_part->setFont(maintFont);
    btn_create_spatial_view_4part->setFont(maintFont);
    btn_create_histogram_view->setFont(maintFont);
    btn_create_plot_view->setFont(maintFont);
    btn_create_table_view->setFont(maintFont);
    btn_create_well_profile_view->setFont(maintFont);
    btn_crate_user_view->setFont(maintFont);


    // Добавление кнопок в сетку
    buttonLayout->addWidget(btn_create_spatial_view_part, 0, 1);
    buttonLayout->addWidget(btn_create_spatial_view_4part, 1, 1);
    buttonLayout->addWidget(btn_create_histogram_view, 2, 1);
    buttonLayout->addWidget(btn_create_plot_view, 3, 1);
    buttonLayout->addWidget(btn_create_table_view, 4, 1);
    buttonLayout->addWidget(btn_create_well_profile_view, 5, 1);
    buttonLayout->addWidget(btn_crate_user_view, 6, 1);

    // Добавление спейсеров в сетку
    buttonLayout->addItem(spacer_left, 0, 0, 7, 1); // Левый спейсер
    buttonLayout->addItem(spacer_right, 0, 2, 7, 1); // Правый спейсер

    // Добавление сетки с кнопками в основной макет
    mainLayout->addLayout(buttonLayout);

    // Добавление нижнего спейсера
    mainLayout->addItem(spacer_bottom);

    // Соединение сигналов и слотов
    connect(btn_create_spatial_view_part, &QPushButton::clicked, this, &AddViewWidget::on_btn_create_spatial_view_part_clicked);
}

AddViewWidget::~AddViewWidget() {
    delete btn_create_spatial_view_part;
    delete btn_create_spatial_view_4part;
    delete btn_create_histogram_view;
    delete btn_create_plot_view;
    delete btn_create_table_view;
    delete btn_create_well_profile_view;
    delete btn_crate_user_view;
    delete label_main;
    delete spacer_top;
    delete spacer_bottom;
    delete spacer_left;
    delete spacer_right;
    delete buttonLayout;
    delete mainLayout;
}

SpatialViewWidget* AddViewWidget::createSpatialViewWidget() {
    SpatialViewWidget* widget = new SpatialViewWidget();
    return widget;
}

void AddViewWidget::on_btn_create_spatial_view_part_clicked() {
    emit createSpatialViewWidgetBtnClicked(createSpatialViewWidget());
}
