#ifndef WELLSDATAWIDGET_H
#define WELLSDATAWIDGET_H

#include <QVBoxLayout>
#include <QWidget>
#include <QSplitter>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QDateEdit>
#include <QDebug>

#include "Eigen/Dense"

#include "../Core/physicalquantity.h"
#include "../Core/wells.h"

#include "wellstreewidget.h"
#include "creatorwidgets.h"
#include "wellcontrolmodel.h"
#include "welltrajectorymodel.h"
#include "perforationmodel.h"
#include "doublevalidatordelegate.h"
#include "doublespinboxdelegate.h"
#include "comboboxdelegate.h"
#include "dateeditdelegate.h"
#include "tableview.h"

class WellsDataWidget : public QWidget {
    Q_OBJECT
public:
    explicit WellsDataWidget(QWidget* parent = nullptr);
    ~WellsDataWidget();
    void openProject(PhysicalQuantity* physicalquantity);

private slots:
    void on_btn_del_row_welltrack_clicked();
    void on_btn_add_row_welltrack_clicked();
    void on_btn_apply_welltracks_clicked();
    void wellAdded(const QString& wellname);
    void wellDeleted(const QString& wellname);
    void changedCurrentWell(const QString& wellname);
    void wellNameChanged(const QString& oldname, const QString& newname);

private:
    QSplitter* spl;
    WellsTreeWidget* wellstreewidget;
    WellTrajectoryModel* trajectoryModel;
    PerforationModel* perforationModel;
    WellControlModel* controlModel;
    QString currentWellName;
    H5FileManager& h5filemanager;

    QTabWidget* tabWidget;
    TableView* trajectoryView;
    TableView* controlView;
    TableView* perforationView;
    TableView* historyView;
    QPushButton* btn_add_row_welltrack;
    QPushButton* btn_del_row_welltrack;
    QPushButton* btn_apply_welltracks;
};

#endif // WELLSDATAWIDGET_H
