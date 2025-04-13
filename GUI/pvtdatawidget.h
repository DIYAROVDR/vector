#ifndef PVTDATAWIDGET_H
#define PVTDATAWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QTableWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QSplitter>
#include <QDebug>
#include <QVBoxLayout>


#include "../Core/pvt.h"
#include "../Core/pvtw.h"
#include "../Core/pvcdo.h"
#include "../Core/density.h"
#include "../Core/physicalquantity.h"

#include "../GUI/ttdelegate.h"
#include "../GUI/treeview.h"
#include "../GUI/tableview.h"
#include "../GUI/chartview.h"
#include "../GUI/pvttreemodel.h"
#include "../GUI/pvttablemodel.h"
#include "../GUI/pvtchartmodel.h"
#include "../GUI/multidelegate.h"


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

#endif // PVTDATAWIDGET_H
