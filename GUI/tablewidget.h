#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QTableWidget>
#include <QHeaderView>
#include <vector>

#include "units.h"

class TableWidget : public QTableWidget {
    Q_OBJECT

public:
    explicit TableWidget(QWidget* parent = nullptr);

    void setData(std::initializer_list<std::vector<double>> data);
    void setHeaders(const std::initializer_list<QString>& headers);
    void setUnits(std::initializer_list<UnitType>);

private:
    std::vector<UnitType> types;
};

#endif // TABLEWIDGET_H
