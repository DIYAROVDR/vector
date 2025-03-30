#ifndef MULTIVIEWIDGET_H
#define MULTIVIEWIDGET_H

#include <QTabWidget>
#include <QTabBar>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

#include "../GUI/addviewwidget.h"
#include "../Core/h5filemanager.h"

class MultiViewWidget : public QTabWidget {
    Q_OBJECT
public:
    explicit MultiViewWidget(QWidget* parent = nullptr);
public slots:
    void loadGrid();
    void updateGrid(size_t step);
    void showAxis(bool flag);
    void showOrientationAxis(bool flag);
    void showLegend(bool flag);

private:
    AddViewWidget* addwgt;
    QFont mainfont;
    void addCloseButton(int index);
};

#endif // MULTIVIEWIDGET_H


/*
#include <vtkSmartPointer.h>
#include <vtkDoubleArray.h>
#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>



vtkSmartPointer<vtkDoubleArray> H5FileManager::loadScalar(size_t step) {
    auto scalarData = getDynamicCube("PRESSURE",step);
    vtkSmartPointer<vtkDoubleArray> vtkScalar = vtkSmartPointer<vtkDoubleArray>::New();

    for (size_t i = 0; i < scalarData.size(); ++i) {
        vtkScalar->InsertNextValue(scalarData[i]);
    }

    return vtkScalar;
}


vtkSmartPointer<vtkStructuredGrid> H5FileManager::loadGrid() {
    vtkSmartPointer<vtkStructuredGrid> vtkgrid = vtkSmartPointer<vtkStructuredGrid>::New();
    vtkSmartPointer<vtkPoints> vtkpoints = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkDoubleArray> vtkarray = vtkSmartPointer<vtkDoubleArray>::New();

    vtkgrid->SetDimensions(nx + 1, ny + 1, nz + 1);
    vtkarray->SetNumberOfComponents(3);
    vtkarray->SetNumberOfTuples((nx + 1)*(ny + 1)*(nz + 1));

    grid.openDataSet("nodes").read(vtkarray->GetPointer(0), H5::PredType::NATIVE_DOUBLE);

    vtkpoints->SetData(vtkarray);
    vtkgrid->SetPoints(vtkpoints);

    return vtkgrid;
}


vtkSmartPointer<vtkUnstructuredGrid> H5FileManager::loadGridCP() {
    vtkSmartPointer<vtkUnstructuredGrid> vtkgrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    vtkSmartPointer<vtkPoints> vtkpoints = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkDoubleArray> vtkarray = vtkSmartPointer<vtkDoubleArray>::New();
    vtkSmartPointer<vtkCellArray> vtkcells = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkIdTypeArray> cellData = vtkSmartPointer<vtkIdTypeArray>::New();

    const size_t zcorn_size = 8 * nx * ny * nz;

    vtkarray->SetNumberOfComponents(3);
    cellData->SetNumberOfComponents(1);
    vtkarray->SetNumberOfTuples(zcorn_size);
    cellData->SetNumberOfTuples(zcorn_size);

    grid.openDataSet("nodes").read(vtkarray->GetPointer(0), H5::PredType::NATIVE_DOUBLE);
    grid.openDataSet("hexahedrons").read(cellData->GetPointer(0), H5::PredType::NATIVE_LLONG);

    vtkpoints->SetData(vtkarray);
    vtkcells->SetData(8,cellData);

    vtkgrid->SetPoints(vtkpoints);
    vtkgrid->SetCells(VTK_HEXAHEDRON, vtkcells);

    return vtkgrid;
}


*/