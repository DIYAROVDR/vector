#include "chartview.h"
#include <QDebug>

ChartView::ChartView(QWidget *parent)
        : QChartView(parent)
{
    // Создаем и настраиваем график
    m_chart = new QChart();
    m_chart->setMargins(QMargins(0, 0, 0, 0));  // Уменьшаем отступы
    m_chart->legend()->setVisible(true);         // Показываем легенду

    // Создаем оси
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();

    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);

    // Устанавливаем график в QChartView
    this->setChart(m_chart);
    this->setRenderHint(QPainter::Antialiasing);  // Включаем сглаживание
}

// Добавление серии данных
void ChartView::addSeries(QAbstractSeries *series)
{
    m_chart->addSeries(series);
    series->attachAxis(m_axisX);
    series->attachAxis(m_axisY);
}

// Установка заголовка графика
void ChartView::setChartTitle(const QString &title)
{
    m_chart->setTitle(title);
}

// Настройка оси X
void ChartView::setAxisX(const QString &title, double min, double max)
{
    m_axisX->setTitleText(title);
    m_axisX->setRange(min, max);
}

// Настройка оси Y
void ChartView::setAxisY(const QString &title, double min, double max)
{
    m_axisY->setTitleText(title);
    m_axisY->setRange(min, max);
}

// Переопределение событий (пример масштабирования)
void ChartView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        qDebug() << "Left mouse pressed at" << event->pos();
    QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    // Можно добавить логику перемещения графика
    QChartView::mouseMoveEvent(event);
}

void ChartView::wheelEvent(QWheelEvent *event)
{
    // Масштабирование колесиком мыши
    if (event->angleDelta().y() > 0)
        m_chart->zoomIn();
    else
        m_chart->zoomOut();

    QChartView::wheelEvent(event);
}