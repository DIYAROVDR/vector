#include "plotwidget.h"


PlotWidget::PlotWidget(QWidget* parent, int graphCount) : QCustomPlot(parent) {
    createGraphs(graphCount);
    setFontSettings();
    setAxisSettings();
    setItemTextSettings();
    setZoomSettings();

    y1Tracer = new QCPItemTracer(this);
    y1Tracer->setStyle(QCPItemTracer::tsCrosshair);
    y1Tracer->setPen(QPen(Qt::green));

    setMouseTracking(true);
    connect(this, &QCustomPlot::mouseMove, this, &PlotWidget::onMouseMove);
    connect(this, &PlotWidget::bothMouseButtonsClicked, this, &PlotWidget::setInitRange);
}


void PlotWidget::setData(const std::vector<std::vector<double>>& data) {
    auto xconv = Units::TS(data.back(), unittypes.back());
    x = QVector<double>(xconv.begin(), xconv.end());

    ydata.resize(data.size() - 1);

    for(int i = 0; i < data.size() - 1; ++i) {
        auto yconv = Units::TS(data[i], unittypes[i]);
        ydata[i] = QVector<double>(yconv.begin(), yconv.end());
        graph(i)->data()->clear();
        graph(i)->addData(x, ydata[i]);
    }

    setInitRange();
}


void PlotWidget::setAxisLabels(const QStringList& labels) {
    const QString labelTemplate = "%1, %2";
    for(int i = 0; i < labels.size() - 1; ++i) {
        graph(0)->setName(labels[i]);
    }

    yAxis->setLabel(labelTemplate.arg(labels[0], Units::unitNameTS(unittypes[0])));

    if(labels.size() == 3) {
        yAxis2->setLabel(labelTemplate.arg(labels[1], Units::unitNameTS(unittypes[1])));
    }

    xAxis->setLabel(labelTemplate.arg(labels.last(), Units::unitNameTS(unittypes.back())));
}


void PlotWidget::setTypeUnitAxis(const std::vector<UnitType>& types) {
    unittypes = types;
}


void PlotWidget::mousePressEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton) && (event->buttons() & Qt::RightButton)) {
        emit bothMouseButtonsClicked();
    }
    QCustomPlot::mousePressEvent(event);
}


void PlotWidget::paintEvent(QPaintEvent *event) {
    QCustomPlot::paintEvent(event);

    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 2));
    painter.drawRect(rect());
}


void PlotWidget::createGraphs(int count) {
    if(count >= 2){
        legend = new QCPLegend();
        legend->setFont(QFont("Segoe", 12));
        axisRect()->insetLayout()->addElement(legend, Qt::AlignTop | Qt::AlignCenter);
    }

    for (int i = 0; i < count; ++i) {
        addGraph();
        if (legend) {
            graph(i)->addToLegend(legend);
        }
        QColor color = QColor::fromRgb(1*i, 100*i, 20*i);
        graph(i)->setPen(QPen(color, 2));
    }
}


void PlotWidget::setFontSettings() {
    QFont labelFont = yAxis->labelFont();
    labelFont.setPointSize(10);
    yAxis->setLabelFont(labelFont);
    yAxis2->setLabelFont(labelFont);
    xAxis->setLabelFont(labelFont);

    QFont tickFont = yAxis->tickLabelFont();
    tickFont.setPointSize(10);
    yAxis->setTickLabelFont(tickFont);
    yAxis2->setTickLabelFont(tickFont);
    xAxis->setTickLabelFont(tickFont);
}


void PlotWidget::setItemTextSettings() {
    cursorLabel = new QCPItemText(this);
    cursorLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignBottom);
    cursorLabel->setFont(QFont(font().family(), 8));
    cursorLabel->setColor(Qt::black);
    cursorLabel->setVisible(true);
}


void PlotWidget::setAxisSettings() {
    yAxis2->setVisible(true);
    xAxis->grid()->setSubGridVisible(true);
    yAxis->grid()->setSubGridVisible(true);
    QPen subGridPen;
    subGridPen.setStyle(Qt::DotLine);
    subGridPen.setColor(QColor(200, 200, 200));
    xAxis->grid()->setSubGridPen(subGridPen);
    yAxis->grid()->setSubGridPen(subGridPen);
}


void PlotWidget::setZoomSettings() {
    setInteraction(QCP::iRangeDrag, true);
    setInteraction(QCP::iRangeZoom, true);
    axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
}


void PlotWidget::setInitRange() {
    rescaleAxes();
    replot();
}


void PlotWidget::onMouseMove(QMouseEvent *event) {
    double x = xAxis->pixelToCoord(event->pos().x());
    double y1 = yAxis->pixelToCoord(event->pos().y());
    double y2 = yAxis2->pixelToCoord(event->pos().y());

    cursorLabel->setText(
        QString("%1: %2\n%3: %4\n%5: %6").
        arg(xAxis->label()).arg(x, 0, 'f', 2).
        arg(yAxis->label()).arg(y1, 0, 'f', 2).
        arg(yAxis2->label()).arg(y2, 0, 'f', 2)
    );

    y1Tracer->position->setCoords(x, y1);
    cursorLabel->position->setCoords(x, y1);
    replot();
}


PlotWidget::~PlotWidget() {

}
