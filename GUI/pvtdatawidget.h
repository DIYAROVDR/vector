#ifndef PVTDATAFORM_H
#define PVTDATAFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QTableWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QSplitter>
#include <QDebug>

#include "units.h"
#include "creatorwidgets.h"
#include "ttdelegate.h"
#include "plotwidget.h"

#include "../Core/pvt.h"
#include "../Core/pvtw.h"
#include "../Core/pvcdo.h"
#include "../Core/density.h"


namespace Ui {
class PVTDataWidget;
}

class PVTDataWidget : public QWidget {
    Q_OBJECT

public:
    explicit PVTDataWidget(QWidget* parent = nullptr);
    void setRegionCount(int count);
    void openProject();
    ~PVTDataWidget();

signals:

private slots:


private:
    int regcount = 1;
    int curreg = 0;
    QFont mainfont;
    QFont boldfont;
};

#endif // PVTDATAFORM_H
