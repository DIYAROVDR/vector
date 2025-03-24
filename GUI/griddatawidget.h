#ifndef GRIDDATAWIDGET_H
#define GRIDDATAWIDGET_H

#include <mkl.h>
#include <Eigen/Sparse>
#include <Eigen/PardisoSupport>
#include <Eigen/IterativeLinearSolvers>

#include <chrono>
#include <thread>

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <vtkDoubleArray.h>
#include <vtkSmartPointer.h>

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QTreeWidget>

#include <numeric>
#include <algorithm>

#include "creatorwidgets.h"
#include "ttdelegate.h"

#include "../Core/h5filemanager.h"
#include "../Core/pvtw.h"
#include "../Core/blockcenteredgrid.h"
#include "../Core/well.h"

#include "../GUI/multidelegate.h"
#include "../GUI/griddatamodel.h"

#include <QVBoxLayout>

class GridDataWidget : public QWidget {
    Q_OBJECT
public:
    explicit GridDataWidget(QWidget* parent = nullptr);
    ~GridDataWidget();
    void setH5FileManager(H5FileManager* h5filemanager);

    Grid* getGrid();
    Well* getWell();

    void openProject();

signals:
    void dimensChanged();


private slots:
    void on_btn_create_grid_clicked();
    void on_btn_export_grid_clicked();
    void on_btn_import_grid_clicked();

private:
    QFont mainFont;
    QFont boldFont;

    GridDataModel* griddatamodel = nullptr;
    H5FileManager& h5filemanager;

    Grid* grid = nullptr;
    Well* well = nullptr;

    int nx = 10;
    int ny = 10;
    int nz = 5;

    QTreeView* treeView;
    void processBlockCenteredGrid(); // Блочно-центрированная геометрия
    void processCornerPointGrid();   // Геометрия угловой точки
    void processVertexDefinedGrid(); // Задание сетки вершинами блоков
    void createGrid();
};



#endif // GRIDDATAWIDGET_H
