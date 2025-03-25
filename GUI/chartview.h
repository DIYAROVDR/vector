#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE  // Используем пространство имен QtCharts

class ChartView : public QChartView
{
Q_OBJECT

public:
    explicit ChartView(QWidget *parent = nullptr);

    // Добавление серии данных
    void addSeries(QAbstractSeries *series);

    // Установка заголовка графика
    void setChartTitle(const QString &title);

    // Настройка осей
    void setAxisX(const QString &title, double min, double max);
    void setAxisY(const QString &title, double min, double max);

protected:
    // Переопределение событий (опционально)
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QChart *m_chart;      // Основной график
    QValueAxis *m_axisX;  // Ось X
    QValueAxis *m_axisY;  // Ось Y
};

#endif // CHARTVIEW_H