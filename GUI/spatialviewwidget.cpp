#include "spatialviewwidget.h"

SpatialViewWidget::SpatialViewWidget(QWidget* parent)
    : QWidget(parent),
    vtkRenderWidget(new QVTKOpenGLNativeWidget(this)),
    renderWindow(vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New()),
    lut(vtkSmartPointer<vtkLookupTable>::New()),
    scalarBar(vtkSmartPointer<vtkScalarBarActor>::New()),
    mapper(vtkSmartPointer<vtkDataSetMapper>::New()),
    renderer(vtkSmartPointer<vtkRenderer>::New()),
    axesActor(vtkSmartPointer<vtkAxesActor>::New()),
    cubeAxesActor(vtkSmartPointer<vtkCubeAxesActor>::New()),
    actorColormap(vtkSmartPointer<vtkActor>::New()),
    actorWireframe(vtkSmartPointer<vtkActor>::New()),
    wireframeMapper(vtkSmartPointer<vtkDataSetMapper>::New()),
    orientationMarkerWidget(vtkSmartPointer<vtkOrientationMarkerWidget>::New()) {

    // Установка OpenGL формата
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

    // Привязка рендер окна к виджету
    vtkRenderWidget->setRenderWindow(renderWindow);

    // Настройка Lookup Table и маппера
    lut->SetNumberOfTableValues(256);
    lut->Build();

    mapper->SetLookupTable(lut);

    // Настройка цветовой карты
    scalarBar->SetLookupTable(lut);
    setLegendSettings();

    // Настройка рендера
    renderer->SetBackground(0.321, 0.341, 0.431);
    renderWindow->AddRenderer(renderer);

    renderWindow->Render();


    // Добавляем рендер-виджет
    vtkRenderWidget->update();

    camera = renderer->GetActiveCamera();

    // Настройка виджета ориентации
    orientationMarkerWidget->SetOrientationMarker(axesActor);
    orientationMarkerWidget->SetInteractor(renderWindow->GetInteractor());
    orientationMarkerWidget->EnabledOn(); // Включаем виджет ориентации
    orientationMarkerWidget->SetViewport(0.0, 0.0, 0.2, 0.2); // Угловое положение (левый нижний угол)
    showOrientationAxis(true);
    //orientationMarkerWidget->InteractiveOff(); // Отключение интерактивности
    setCubeAxisSettings();
    showAxis(true);
}

void SpatialViewWidget::setScalar(vtkSmartPointer<vtkDoubleArray> scalar) {
    double range[2];
    scalar->GetRange(range);  // Получаем диапазон значений

    // Обновляем Lookup Table
    lut->SetTableRange(range);
    lut->Build();

    // Обновляем маппер
    mapper->SetScalarRange(range);

    // Устанавливаем скалярные данные для CellData
    grid->GetCellData()->SetScalars(scalar);

    // Обновляем отображение
    renderWindow->Render();
    vtkRenderWidget->update();
}

void SpatialViewWidget::createGrid(vtkSmartPointer<vtkStructuredGrid> grid, vtkSmartPointer<vtkDoubleArray> scalar) {
    this->grid = grid;
    grid->GetCellData()->SetScalars(scalar);

    // Настройка Lookup Table и маппера
    lut->SetNumberOfTableValues(256);

    double range[2];
    scalar->GetRange(range);  // Получаем диапазон значений
    lut->SetTableRange(range);
    lut->SetHueRange(0.666, 0.0);  // От синего к красному
    lut->Build();

    mapper->SetInputData(grid);
    mapper->SetLookupTable(lut);
    mapper->SetScalarRange(range);

    // Настройка акторов
    actorColormap->SetMapper(mapper);
    actorWireframe->SetMapper(wireframeMapper);
    wireframeMapper->SetInputData(grid);

    // Настройка цветовой карты
    scalarBar->SetLookupTable(lut);
    scalarBar->SetLabelFormat("%0.1f");
    scalarBar->GetLabelTextProperty()->SetFontSize(15);  // Устанавливаем размер шрифта
    setLegendSettings();

    // Настройка сетки (каркаса)
    setGridSettings();
    updateBounds(grid);
    // Добавляем акторы в рендерер
    renderer->AddActor(actorColormap);   // Актор с цветовой картой
    renderer->AddActor(actorWireframe);  // Актор с каркасом
    renderer->SetBackground(0.321, 0.341, 0.431);

    // Добавляем рендерер в окно
    renderWindow->AddRenderer(renderer);

    // Отображение
    renderWindow->Render();
    vtkRenderWidget->update();
    //addWell(1,1);
}

void SpatialViewWidget::addWell(int i, int j) {
    if (!grid) {
        qWarning("Grid is not initialized!");
        return;
    }

    vtkSmartPointer<vtkPoints> points = grid->GetPoints();
    if (!points) {
        qWarning("Grid has no points!");
        return;
    }

    int dims[3];
    vtkStructuredGrid::SafeDownCast(grid)->GetDimensions(dims);

    if (i < 0 || i >= dims[0] - 1 || j < 0 || j >= dims[1] - 1) {
        qWarning("Index out of bounds!");
        return;
    }

    // Получаем координаты центра ячейки
    vtkIdType id1 = i + j * dims[0];
    vtkIdType id2 = id1 + 1;
    double p1[3], p2[3];
    points->GetPoint(id1, p1);
    points->GetPoint(id2, p2);

    double center[3] = {
        (p1[0] + p2[0]) / 2.0,
        (p1[1] + p2[1]) / 2.0,
        (p1[2] + p2[2]) / 2.0
    };

    double cellSize = std::abs(p2[0] - p1[0]);
    double radius = cellSize * 0.3;
    double height = 10.0;

    vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
    cylinder->SetRadius(radius);
    cylinder->SetHeight(height);
    cylinder->SetResolution(20);
    cylinder->Update();

    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->RotateX(90);

    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformFilter->SetTransform(transform);
    transformFilter->SetInputConnection(cylinder->GetOutputPort());
    transformFilter->Update();

    vtkSmartPointer<vtkPolyDataMapper> wellMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    wellMapper->SetInputConnection(transformFilter->GetOutputPort());

    vtkSmartPointer<vtkActor> wellActor = vtkSmartPointer<vtkActor>::New();
    wellActor->SetMapper(wellMapper);
    wellActor->SetPosition(center[0], center[1], center[2] - height / 2.0);
    wellActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

    renderer->AddActor(wellActor);
    renderWindow->Render();
}



void SpatialViewWidget::resizeEvent(QResizeEvent* event) {
    vtkRenderWidget->setFixedSize(this->width(), this->height());
    QWidget::resizeEvent(event);
}

void SpatialViewWidget::mousePressEvent(QMouseEvent *event) {
    // Проверяем, что одновременно нажаты ЛКМ и ПКМ
    if (event->buttons() & Qt::LeftButton && event->buttons() & Qt::RightButton) {

        // Если объект существует
        if (grid) {
            double bounds[6];
            grid->GetBounds(bounds);  // Получаем границы объекта

            // Находим центр объекта
            double center[3] = {
                (bounds[0] + bounds[1]) / 2.0,
                (bounds[2] + bounds[3]) / 2.0,
                (bounds[4] + bounds[5]) / 2.0
            };

            // Получаем камеру рендерера

            // Устанавливаем позицию камеры
            double distance = 10.0;  // Расстояние от центра
            camera->SetPosition(center[0], center[1], center[2] + distance); // Позиция камеры
            camera->SetFocalPoint(center[0], center[1], center[2]); // Точка фокусировки камеры

            // Обновляем камеру и рендер
            renderer->ResetCamera();
            renderWindow->Render();
        }
    }
}

void SpatialViewWidget::mouseMoveEvent(QMouseEvent *event) {

}

void SpatialViewWidget::mouseReleaseEvent(QMouseEvent *event) {

}

void SpatialViewWidget::setLegendSettings() {
    scalarBar->GetLabelTextProperty()->SetColor(1.0, 1.0, 1.0);  // Белый цвет
    scalarBar->SetNumberOfLabels(10);

    vtkTextProperty* labelTextProperty = scalarBar->GetLabelTextProperty();
    labelTextProperty->SetFontSize(12);
    labelTextProperty->SetFontFamilyToArial();
    labelTextProperty->SetJustificationToCentered();
    labelTextProperty->BoldOn();
    labelTextProperty->ItalicOff();

    vtkTextProperty* titleTextProperty = scalarBar->GetTitleTextProperty();
    titleTextProperty->SetColor(1.0, 1.0, 1.0);
    titleTextProperty->SetFontSize(10);
    titleTextProperty->BoldOn();

    scalarBar->SetHeight(0.3);  // 30% от высоты окна
    scalarBar->SetWidth(0.05);  // 5% от ширины окна
    scalarBar->SetPosition(0.9, 0.1);  // Справа внизу

    renderer->AddActor2D(scalarBar);  // Легенда
}

void SpatialViewWidget::setGridSettings() {
    actorWireframe->GetProperty()->SetRepresentationToWireframe();
    actorWireframe->GetProperty()->SetColor(0.0, 0.0, 0.0);  // Черный цвет каркаса
    actorWireframe->GetProperty()->SetLineWidth(1.0);
}

void SpatialViewWidget::setCubeAxisSettings() {
    double bounds[6] = {0.0, 10.0, 0.0, 10.0, 0.0, 10.0}; // Устанавливаем границы по всем осям

    // Установка подписей осей
    cubeAxesActor->SetXTitle("X Axis");
    cubeAxesActor->SetYTitle("Y Axis");
    cubeAxesActor->SetZTitle("Z Axis");

    // Установка камеры и оформления
    cubeAxesActor->SetCamera(renderer->GetActiveCamera());
    cubeAxesActor->GetTitleTextProperty(0)->SetColor(1, 1, 1); // Белый цвет заголовков
    cubeAxesActor->GetLabelTextProperty(0)->SetColor(1, 1, 1); // Белый цвет подписей
    cubeAxesActor->GetLabelTextProperty(1)->SetColor(1, 1, 1);
    cubeAxesActor->GetLabelTextProperty(2)->SetColor(1, 1, 1);

    // Установка границ
    cubeAxesActor->SetBounds(bounds);
}


void SpatialViewWidget::showAxis(bool flag) {
    if (flag) {
        renderer->AddActor(cubeAxesActor);
    } else {
        renderer->RemoveActor(cubeAxesActor);
    }

    renderWindow->Render();
}

void SpatialViewWidget::showOrientationAxis(bool flag) {
    if (axesActor) {
        axesActor->SetVisibility(flag ? 1 : 0);
        renderWindow->Render();
    }
}

void SpatialViewWidget::showLegend(bool flag) {
    if (flag) {
        // Add the scalar bar (legend) to the renderer
        renderer->AddActor2D(scalarBar);
    } else {
        // Remove the scalar bar (legend) from the renderer
        renderer->RemoveActor2D(scalarBar);
    }

    // Update the render window
    renderWindow->Render();
}

void SpatialViewWidget::createGrid(vtkSmartPointer<vtkDataSet> grid) {

}

void SpatialViewWidget::createGrid(vtkSmartPointer<vtkUnstructuredGrid> grid) {
    this->grid = grid;  // Обновляем текущий указатель на сетку
    mapper->SetInputData(grid);
    actorColormap->SetMapper(mapper);
    wireframeMapper->SetInputData(grid);
    actorWireframe->SetMapper(wireframeMapper);

    // Добавляем акторы в рендерер
    renderer->AddActor(actorColormap);   // Актор с цветовой картой
    renderer->AddActor(actorWireframe);  // Актор с каркасом
    updateBounds(grid);
}

void SpatialViewWidget::createGrid(vtkSmartPointer<vtkStructuredGrid> grid) {
    this->grid = grid;
    mapper->SetInputData(grid);
    actorColormap->SetMapper(mapper);
    wireframeMapper->SetInputData(grid);
    actorWireframe->SetMapper(wireframeMapper);
    // Добавляем акторы в рендерер
    renderer->AddActor(actorColormap);   // Актор с цветовой картой
    renderer->AddActor(actorWireframe);  // Актор с каркасом

    updateBounds(grid);


    renderWindow->Render();
    vtkRenderWidget->update();
}

void SpatialViewWidget::updateBounds(vtkSmartPointer<vtkDataSet> grid) {
    double bounds[6];
    grid->GetBounds(bounds);

    for (int i = 0; i < 3; ++i) {
        double center = (bounds[2 * i] + bounds[2 * i + 1]) / 2.0; // Центр диапазона
        double halfRange = (bounds[2 * i + 1] - bounds[2 * i]) / 2.0; // Половина диапазона
        bounds[2 * i] = center - scaleFactor * halfRange;           // Новый минимум
        bounds[2 * i + 1] = center + scaleFactor * halfRange;       // Новый максимум
    }

    cubeAxesActor->SetBounds(bounds);
    setGridSettings();
    renderWindow->Render();
    vtkRenderWidget->update();
}

