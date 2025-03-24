#ifndef SPATIALVIEWWIDGET_H
#define SPATIALVIEWWIDGET_H

#include <QWidget>
#include <QPointer>
#include <QMouseEvent>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkUnstructuredGrid.h>
#include <vtkStructuredGrid.h>
#include <vtkScalarBarActor.h>
#include <vtkDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkNew.h>
#include <vtkPoints.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkProperty.h>
#include <vtkAxesActor.h>
#include <QSurfaceFormat>

#include <vtkCamera.h>

#include <vtkLookupTable.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkTextProperty.h>

#include <vtkDataSetMapper.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkTextActor.h>
#include <vtkCubeAxesActor.h>
#include <vtkCaptionActor2D.h>

#include <vtkCellData.h>


#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>


class SpatialViewWidget : public QWidget {
    Q_OBJECT
public:
    SpatialViewWidget(QWidget* parent = nullptr);

    void createGrid(vtkSmartPointer<vtkDataSet> grid);
    void createGrid(vtkSmartPointer<vtkStructuredGrid> grid);
    void createGrid(vtkSmartPointer<vtkUnstructuredGrid> grid);
    void setScalar(vtkSmartPointer<vtkDoubleArray> scalar);
    void createGrid(vtkSmartPointer<vtkStructuredGrid> grid, vtkSmartPointer<vtkDoubleArray> scalar);
    void addWell(int i, int j);

public slots:
    void showAxis(bool flag);
    void showOrientationAxis(bool flag);
    void showLegend(bool flag);

private:
    double scaleFactor = 1.25;
    bool isMousePressed = false;  // To check if mouse button is pressed
    QPoint lastMousePos;         // To track the last mouse position
    vtkSmartPointer<vtkCamera> camera; // Camera pointer to manipulate the camera position and orientation
    QPointer<QVTKOpenGLNativeWidget> vtkRenderWidget;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    //vtkSmartPointer<vtkStructuredGrid> grid;
    vtkSmartPointer<vtkDataSet> grid;
    vtkSmartPointer<vtkLookupTable> lut;
    vtkSmartPointer<vtkScalarBarActor> scalarBar;
    vtkSmartPointer<vtkDataSetMapper> mapper;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkAxesActor> axesActor;
    vtkSmartPointer<vtkCubeAxesActor> cubeAxesActor;
    vtkSmartPointer<vtkActor> actorColormap;
    vtkSmartPointer<vtkActor> actorWireframe;
    vtkSmartPointer<vtkDataSetMapper> wireframeMapper;
    vtkSmartPointer<vtkOrientationMarkerWidget> orientationMarkerWidget;

    void setLegendSettings();
    void setGridSettings();
    void setCubeAxisSettings();
    void updateBounds(vtkSmartPointer<vtkDataSet> grid);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};

#endif // SPATIALVIEWWIDGET_H
