#ifndef TIMESLIDERWIDGET_H
#define TIMESLIDERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>

class TimeSliderWidget : public QWidget {
    Q_OBJECT
public:
    explicit TimeSliderWidget(QWidget* parent = nullptr);
    ~TimeSliderWidget();
    void updateTimeStep();
    void setLastStep(size_t step);
    void stop();

signals:
    void btn_run_clicked();
    void sp_timestep_changed(int step);

private slots:
    void on_btn_run_clicked();
    void on_horizontalSlider_valueChanged(int value);

private:
    size_t step = 0;
    QPushButton* btn_run;
    QSlider* horizontalSlider;
    QSpinBox* spinBox;
    QComboBox* comboBox;
    QPushButton* btn_settings;
};

#endif // TIMESLIDERWIDGET_H
