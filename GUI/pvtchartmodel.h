#ifndef PVTCHARTMODEL_H
#define PVTCHARTMODEL_H

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QList>

QT_CHARTS_USE_NAMESPACE

class PVTTableModel;

class PVTChartModel : public QChart {
    Q_OBJECT
public:
    explicit PVTChartModel(QGraphicsItem* parent = nullptr);
    ~PVTChartModel();

    void setTableModel(PVTTableModel* model);
    void refreshChart();

private:
    void clearChart();
    QValueAxis* createAdditionalAxis(int colIndex);

    PVTTableModel* m_tableModel;
    QList<QLineSeries*> m_series;
    QValueAxis* m_axisX;
    QList<QValueAxis*> m_additionalAxes; // Дополнительные оси
};

#endif // PVTCHARTMODEL_H
