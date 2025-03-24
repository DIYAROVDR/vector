#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include <QMouseEvent>

#include "qcustomplot.h"
#include "units.h"

class PlotWidget : public QCustomPlot {
    Q_OBJECT

public:
    explicit PlotWidget(QWidget *parent = nullptr, int graphCount = 1);
    ~PlotWidget();

    void setData(const std::vector<std::vector<double>>& data);
    void setAxisLabels(const QStringList& labels);
    void setTypeUnitAxis(const std::vector<UnitType>& types);

signals:
    void bothMouseButtonsClicked();

private slots:
    void onMouseMove(QMouseEvent* event);
    void setInitRange();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<double> x;
    QVector<QVector<double>> ydata;
    std::vector<UnitType> unittypes;
    QCPItemText* cursorLabel = nullptr;
    QCPLegend* legend = nullptr;
    QCPItemTracer* y1Tracer;

    void createGraphs(int count);
    void setFontSettings();
    void setItemTextSettings();
    void setAxisSettings();
    void setZoomSettings();
};

#endif // PLOTWIDGET_H
