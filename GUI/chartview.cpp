#include "chartview.h"

ChartView::ChartView(QWidget* parent) :
            QChartView(parent),
            m_isDragging(false) {
    setRenderHint(QPainter::Antialiasing);
    setRubberBand(QChartView::RectangleRubberBand);
}

ChartView::ChartView(QChart* chart, QWidget* parent) :
            QChartView(chart, parent),
            m_isDragging(false) {
    setRenderHint(QPainter::Antialiasing);
    setRubberBand(QChartView::RectangleRubberBand);
}

template<typename T, typename... Args>
T* ChartView::createAndSetChart(Args&&... args) {
    static_assert(std::is_base_of<QChart, T>::value,"T must be a subclass of QChart");

    T* chart = new T(std::forward<Args>(args)...);
    setChart(chart);
    return chart;
}

void ChartView::setChart(QChart* chart) {
    QChartView::setChart(chart);
    if (chart) {
        chart->setAcceptHoverEvents(true);
        chart->legend()->setVisible(true);
    }
}

void ChartView::updateChart() {
    if (chart()) {
        chart()->update();
        viewport()->update();
    }
}

void ChartView::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_lastMousePos = event->pos();
        m_isDragging = true;
    }
    QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent* event) {
    if (m_isDragging && chart()) {
        auto dPos = event->pos() - m_lastMousePos;
        chart()->scroll(-dPos.x(), dPos.y());
        m_lastMousePos = event->pos();
    }
    QChartView::mouseMoveEvent(event);
}

void ChartView::wheelEvent(QWheelEvent* event) {
    if (chart()) {
        const double factor = 1.1;
        if (event->angleDelta().y() > 0) {
            chart()->zoom(factor);
        } else {
            chart()->zoom(1.0 / factor);
        }
    }
    QChartView::wheelEvent(event);
}