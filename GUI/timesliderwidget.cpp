#include "timesliderwidget.h"

TimeSliderWidget::TimeSliderWidget(QWidget *parent) : QWidget(parent) {
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,5,0);
    btn_run = new QPushButton(this);
    horizontalSlider = new QSlider(Qt::Horizontal, this);
    spinBox = new QSpinBox(this);
    comboBox = new QComboBox(this);
    btn_settings = new QPushButton(this);

    btn_run->setIcon(QIcon(":/images/run.png"));
    btn_settings->setIcon(QIcon(":/images/settings.png"));

    btn_run->setIconSize(QSize(32,32));
    btn_settings->setIconSize(QSize(32,32));

    layout->addWidget(btn_settings);
    layout->addWidget(btn_run);
    layout->addWidget(spinBox);
    layout->addWidget(comboBox);
    layout->addWidget(horizontalSlider);

    setLayout(layout);

    connect(btn_run, &QPushButton::clicked, this, &TimeSliderWidget::on_btn_run_clicked);
    connect(horizontalSlider, &QSlider::valueChanged, this, &TimeSliderWidget::on_horizontalSlider_valueChanged);

    // Установка стилей для QSlider
    horizontalSlider->setStyleSheet(
        "QSlider::groove:horizontal {"
        "    border: 1px solid #9e9e9e;"
        "    background: #e0e0e0;"
        "    height: 6px;"
        "    border-radius: 2px;"
        "}"
        "QSlider::handle:horizontal {"
        "    background: #3a3a3a;"
        "    border: 1px solid #5e5e5e;"
        "    width: 10px;"
        "    height: 20px;"
        "    margin: -5px 0;"
        "    border-radius: 1px;"
        "}"
        "QSlider::sub-page:horizontal {"
        "    background: #3a3a3a;"
        "    border: 1px solid #5e5e5e;"
        "    height: 10px;"
        "    border-radius: 3px;"
        "}"
        "QSlider::add-page:horizontal {"
        "    background: #e0e0e0;"
        "    border: 1px solid #9e9e9e;"
        "    height: 10px;"
        "    border-radius: 2px;"
        "}"
        );

    // Установка кнопок в плоский стиль
    btn_run->setFlat(true);
    btn_settings->setFlat(true);
}

TimeSliderWidget::~TimeSliderWidget() {
    // Деструктор
}

void TimeSliderWidget::updateTimeStep() {
    ++step;
    horizontalSlider->setValue(step);
    spinBox->setValue(step);
}

void TimeSliderWidget::setLastStep(size_t step) {
    horizontalSlider->setMaximum(step);
}

void TimeSliderWidget::stop() {
    horizontalSlider->setEnabled(true);
    btn_run->setChecked(false);
}

void TimeSliderWidget::on_btn_run_clicked() {
    if (btn_run->isChecked()) {
        horizontalSlider->setEnabled(false);
    } else {
        horizontalSlider->setEnabled(true);
    }
    emit btn_run_clicked();
}

void TimeSliderWidget::on_horizontalSlider_valueChanged(int value) {
    if (!btn_run->isChecked()) {
        emit sp_timestep_changed(value);
    }
}
