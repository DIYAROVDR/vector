#include "pvtchartmodel.h"
#include "pvttablemodel.h"
#include <QDebug>

PVTChartModel::PVTChartModel(QGraphicsItem* parent) :
    QChart(parent),
    m_tableModel(nullptr),
    m_axisX(new QValueAxis())
{
    setTitle("PVT Данные");
    legend()->setVisible(true);
    legend()->setAlignment(Qt::AlignTop);
}

PVTChartModel::~PVTChartModel() {
    clearChart();
    delete m_axisX;
}

void PVTChartModel::setTableModel(PVTTableModel* model) {
    if (m_tableModel != model) {
        if (m_tableModel) {
            disconnect(m_tableModel, &PVTTableModel::dataChanged,
                     this, &PVTChartModel::refreshChart);
        }
        m_tableModel = model;
        if (m_tableModel) {
            connect(m_tableModel, &PVTTableModel::dataChanged,
                   this, &PVTChartModel::refreshChart);
            refreshChart();
        }
    }
}

void PVTChartModel::refreshChart() {
    clearChart();
    if (!m_tableModel || m_tableModel->rowCount() < 1 || m_tableModel->columnCount() < 2)
        return;

    // Настройка основной оси X
    QString xTitle = m_tableModel->headerData(0, Qt::Horizontal).toString();
    m_axisX->setTitleText(xTitle.isEmpty() ? "X-ось" : xTitle);
    m_axisX->setLabelFormat("%.2f");
    addAxis(m_axisX, Qt::AlignBottom);

    // Создаем серии для каждого столбца (начиная со второго)
    for (int col = 1; col < m_tableModel->columnCount(); ++col) {
        QString seriesName = m_tableModel->headerData(col, Qt::Horizontal).toString();
        QLineSeries* series = new QLineSeries();
        series->setName(seriesName);

        // Заполняем данными
        bool hasData = false;
        for (int row = 0; row < m_tableModel->rowCount(); ++row) {
            double x = m_tableModel->data(m_tableModel->index(row, 0)).toDouble();
            double y = m_tableModel->data(m_tableModel->index(row, col)).toDouble();
            if (!qIsNaN(x) && !qIsNaN(y)) {
                series->append(x, y);
                hasData = true;
            }
        }

        if (!hasData) {
            delete series;
            continue;
        }

        addSeries(series);
        m_series.append(series);

        // Привязываем к осям
        QValueAxis* yAxis = createAdditionalAxis(col);
        series->attachAxis(m_axisX);
        series->attachAxis(yAxis);
    }

    zoomReset();
}

QValueAxis* PVTChartModel::createAdditionalAxis(int colIndex) {
    QValueAxis* axis = new QValueAxis(this);
    QString title = m_tableModel->headerData(colIndex, Qt::Horizontal).toString();
    axis->setTitleText(title.isEmpty() ? QString("Y%1-ось").arg(colIndex) : title);
    axis->setLabelFormat("%.2f");

    // Чередуем смещение осей: влево, вправо, влево и т.д.
    Qt::Alignment alignment = (colIndex % 2) ? Qt::AlignRight : Qt::AlignLeft;

    // Добавляем смещение для лучшей читаемости
    if (alignment == Qt::AlignLeft) {
        axis->setTitleBrush(QBrush(Qt::blue)); // Пример стилизации
    } else {
        axis->setTitleBrush(QBrush(Qt::green));
    }

    addAxis(axis, alignment);
    m_additionalAxes.append(axis);

    return axis;
}

void PVTChartModel::clearChart() {
    // Удаляем все серии
    for (QLineSeries* series : m_series) {
        removeSeries(series);
        delete series;
    }
    m_series.clear();

    // Удаляем дополнительные оси
    for (QValueAxis* axis : m_additionalAxes) {
        removeAxis(axis);
        delete axis;
    }
    m_additionalAxes.clear();
}
