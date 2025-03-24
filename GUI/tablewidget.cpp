#include "tablewidget.h"

TableWidget::TableWidget(QWidget *parent) : QTableWidget(parent) {
    setEditTriggers(EditTrigger::NoEditTriggers);
    // setAlternatingRowColors(true);
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->setVisible(false);
    setSelectionMode(QAbstractItemView::NoSelection);
    setSelectionBehavior(QAbstractItemView::SelectItems);
}

void TableWidget::setData(std::initializer_list<std::vector<double>> data) {
    int rowCount = 0;
    for (const auto& vec : data) {
        if (vec.size() > rowCount) {
            rowCount = vec.size();
        }
    }
    setRowCount(rowCount);

    int col = 0;
    for (const auto& vec : data) {
        QVector<double> convertedData = QVector<double>::fromStdVector(Units::TS(vec, types[col]));
        for (int row = 0; row < convertedData.size(); ++row) {
            QTableWidgetItem *item = new QTableWidgetItem(QString::number(convertedData[row]));
            setItem(row, col, item);
        }
        ++col;
    }
}


void TableWidget::setHeaders(const std::initializer_list<QString>& headers) {
    int colCount = headers.size();
    setColumnCount(colCount);

    int col = 0;
    for (const QString &header : headers) {
        QString unitName = Units::unitNameTS(types[col]);
        QString headerWithUnit = QString("%1, %2").arg(header, unitName);
        setHorizontalHeaderItem(col, new QTableWidgetItem(headerWithUnit));
        ++col;
    }
}


void TableWidget::setUnits(std::initializer_list<UnitType> unitTypes) {
    types = std::vector<UnitType>(unitTypes);
}
