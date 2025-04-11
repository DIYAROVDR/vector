#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QtCharts/QAbstractSeries>

QT_CHARTS_USE_NAMESPACE  // Используем пространство имен QtCharts

#include <QtCharts/QChartView>
#include <QtCharts/QChart>

class ChartView : public QChartView {
    Q_OBJECT
public:
    explicit ChartView(QWidget *parent = nullptr);
    explicit ChartView(QChart *chart, QWidget *parent = nullptr);

    template<typename T, typename... Args>
    T* createAndSetChart(Args&&... args);

    void setChart(QChart *chart);

    void updateChart(); // Обновление отображения

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QPointF m_lastMousePos;
    bool m_isDragging;
};

#endif // CHARTVIEW_H