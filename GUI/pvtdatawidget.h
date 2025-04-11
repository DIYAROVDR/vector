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
#include "../Core/physicalquantity.h"

#include "treeview.h"
#include "tableview.h"
#include "chartview.h"
#include "pvttreemodel.h"
#include "pvttablemodel.h"
#include "pvtchartmodel.h"
#include "multidelegate.h"


class PVTDataWidget : public QWidget {
    Q_OBJECT

public:
    explicit PVTDataWidget(QWidget* parent, PhysicalQuantity* physicalquantity);
    void openProject();
    ~PVTDataWidget();

signals:

private slots:
    void onTreeViewItemClicked(const QModelIndex& index);

private:
    QSplitter* splmain;
    QSplitter* spldata;
    TreeView* treeView;
    TableView* tableView;
    ChartView* charView;
    PVTTreeModel* pvtTreeModel;
    PVTTableModel* pvtTableModel;
    PVTChartModel* pvtChartModel;
};

#endif // PVTDATAFORM_H
