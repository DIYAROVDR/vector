#ifndef GRIDDATAWIDGET_H
#define GRIDDATAWIDGET_H

#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>

#include "../Core/h5filemanager.h"
#include "../Core/blockcenteredgrid.h"
#include "../Core/well.h"

#include "../GUI/multidelegate.h"
#include "../GUI/griddatamodel.h"
#include "../GUI/treeview.h"


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
    void on_btn_apply_clicked();
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

    TreeView* treeView;
    QHBoxLayout* btnLayout;
    QPushButton* btnApply;
    QPushButton* btnImport;
    QPushButton* btnExport;
    void processBlockCenteredGrid(); // Блочно-центрированная геометрия
    void processCornerPointGrid();   // Геометрия угловой точки
    void processVertexDefinedGrid(); // Задание сетки вершинами блоков
    void createGrid();
};



#endif // GRIDDATAWIDGET_H
