#ifndef PROJECTDATAWIDGET_H
#define PROJECTDATAWIDGET_H

#include <QTreeWidget>
#include <QObject>
#include <QWidget>
#include <QFont>
#include <QHeaderView>
#include <QLabel>
#include <QDebug>
#include <QStackedWidget>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>

#include "../GUI/pvtdatawidget.h"
#include "../GUI/rockdatawidget.h"
#include "../GUI/griddatawidget.h"
#include "../GUI/genpropwidget.h"
#include "../GUI/initdatawidget.h"
#include "../GUI/fillcubewidget.h"
#include "../GUI/wellsdatawidget.h"
#include "../Core/h5filemanager.h"

class ProjectDataWidget : public QWidget {
    Q_OBJECT
public:
    ProjectDataWidget(QWidget* parent = nullptr);
    void initialization();
    Grid* getGrid();
    Well* getWell();
signals:
    void setCurrentIndexDataWidget(int index);
    void updateGrid();
    void openProject();
public slots:
    void createProject(const QString& dir);
    void updateData();
private slots:
    void handleItemClicked(QTreeWidgetItem* item);
    void openProjectDir();
private:
    QString projectPath;
    QString hdf5FilePath;
    QString fileName;
    QFont mainFont;
    QFont boldFont;

    QHBoxLayout* layout;
    QTreeWidget* treeWidget;
    QStackedWidget* stackedWidget;
    QSplitter* spl;

    PhysicalQuantity* physicalquantity;

    GenPropWidget* genpropwidget;
    GridDataWidget* griddatawidget;
    PVTDataWidget* pvtdatawidget;
    RockDataWidget* rockdatawidget;
    InitDataWidget* initdatawidget;
    FillCubeWidget* fillcubewidget;
    WellsDataWidget* wellsdatawidget;

    H5FileManager& h5filemanager;


    QTreeWidgetItem* headerWidgetItem = nullptr;
    QTreeWidgetItem* modelPropItem = nullptr;
    QTreeWidgetItem* genPropItem = nullptr;
    QTreeWidgetItem* gridItem = nullptr;
    QTreeWidgetItem* pvtItem = nullptr;
    QTreeWidgetItem* rpcItem = nullptr;
    QTreeWidgetItem* rockItem = nullptr;
    QTreeWidgetItem* initStateItem = nullptr;
    QTreeWidgetItem* wellsDataItem = nullptr;
    QTreeWidgetItem* cubesItem = nullptr;
private:
    void initTreeWidget();
    QWidget* createHeaderWidget();
};

#endif // PROJECTDATAWIDGET_H
