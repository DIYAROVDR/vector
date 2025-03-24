#include "griddatawidget.h"

GridDataWidget::GridDataWidget(QWidget* parent):
                QWidget(parent),
                treeView(new TreeView(this)),
                btnLayout(new QHBoxLayout()),
                btnApply(new QPushButton("Применить",this)),
                btnImport(new QPushButton("Импорт", this)),
                btnExport(new QPushButton("Экспорт", this)),
                h5filemanager(H5FileManager::instance()) {

    mainFont.setPointSize(10);
    boldFont.setBold(true);
    btnLayout->setSpacing(5);
    setLayout(new QVBoxLayout(this));
    layout()->addWidget(treeView);
    layout()->addItem(btnLayout);
    layout()->setContentsMargins(5,0,5,0);

    btnLayout->addWidget(btnExport);
    btnLayout->addWidget(btnImport);
    btnLayout->addWidget(btnApply);

    griddatamodel = new GridDataModel();
    treeView->setModel(griddatamodel);
    treeView->setItemDelegateForColumn(1, new MultiDelegate());
    treeView->expandAll();

    connect(griddatamodel,&GridDataModel::dimensChanged,this,[&](){emit dimensChanged();});
    connect(btnApply, &QPushButton::clicked, this, &GridDataWidget::on_btn_apply_clicked);
}

GridDataWidget::~GridDataWidget() {

}


Grid *GridDataWidget::getGrid() {
    return grid;
}


Well *GridDataWidget::getWell() {
    return well;
}


void GridDataWidget::openProject() {

}


void GridDataWidget::on_btn_apply_clicked() {
    griddatamodel->applyData();
    /*
    const int nx_n = nx + 1;
    const int ny_n = ny + 1;
    const int nz_n = nz + 1;

    Eigen::ArrayXi i = Eigen::ArrayXi::LinSpaced(nx_n, 0, nx_n - 1).replicate(ny_n * nz_n, 1);
    Eigen::ArrayXi j = Eigen::ArrayXi::LinSpaced(nx_n * ny_n, 0, ny_n - 1).replicate(nz_n, 1);
    Eigen::ArrayXi k = Eigen::ArrayXi::LinSpaced(nx_n * ny_n * nz_n, 0, nz_n - 1);

    Eigen::ArrayXi index = (i < nx).select(i, i - 1) + nx * ((j < ny).select(j, j - 1) + ny * (k < nz).select(k, k - 1));

    Eigen::VectorXd dx = h5filemanager.getStaticCube("DX");
    Eigen::VectorXd dy = h5filemanager.getStaticCube("DY");
    Eigen::VectorXd dz = h5filemanager.getStaticCube("DZ");

    Eigen::VectorXd x = Eigen::VectorXd::Zero(nx_n * ny_n * nz_n);
    Eigen::VectorXd y = Eigen::VectorXd::Zero(nx_n * ny_n * nz_n);
    Eigen::VectorXd z = Eigen::VectorXd::Zero(nx_n * ny_n * nz_n);

    double x_old = 0.0, y_old = 0.0, z_old = 0.0;
    int index_old = 0, j_old = -1, k_old = -1;

    std::transform(i.data(), i.data() + i.size(), index.data(), x.data(),[&dx, &index_old, &x_old](int i, int idx) {
        if (i == 0) return x_old = 0.0;
        if (idx == index_old) return x_old += dx[idx];
        index_old = idx;
        return x_old = dx[idx] + x_old;
    });

    std::transform(j.data(), j.data() + j.size(), index.data(), y.data(),[&dy, &j_old, &y_old](int j, int idx) {
        if (j == j_old) return y_old;
        j_old = j;
        return y_old = (j == 0) ? 0.0 : dy[idx] + y_old;
    });

    std::transform(k.data(), k.data() + k.size(), index.data(), z.data(), [&dz, &k_old, &z_old](int k, int idx) {
        if (k == k_old) return z_old;
        k_old = k;
        return z_old = (k == 0) ? 0.0 : dz[idx] + z_old;
    });

    Eigen::MatrixXd nodes(nx_n * ny_n * nz_n, 3);

    nodes << x, y, z;

    h5filemanager.setNodes(nodes);
    */
}


void GridDataWidget::on_btn_export_grid_clicked() {
    /*
    grid = new BlockCenteredGrid();
    grid->setDimens(nx,ny,nz);

    Eigen::VectorXd P(nx*ny*nz);
    P.setConstant(200E+5);

    Eigen::ArrayXd dx = h5filemanager.getStaticCube("DX");
    Eigen::ArrayXd dy = h5filemanager.getStaticCube("DY");
    Eigen::ArrayXd dz = h5filemanager.getStaticCube("DZ");

    Eigen::VectorXd kx(nx*ny*nz);
    Eigen::VectorXd ky(nx*ny*nz);
    Eigen::VectorXd kz(nx*ny*nz);

    kx.setConstant(100E-15);
    ky.setConstant(100E-15);
    kz.setConstant(100E-15);

    Eigen::VectorXd poro(nx*ny*nz);
    poro.setConstant(0.2);

    Eigen::VectorXi regions(nx*ny*nz);
    regions.setConstant(1);

    PVT pvt;
    pvt.setPVT(0,new PVTW());
    pvt.setRegions(regions);

    Rock rock;
    rock.setRock(0,RockData());
    rock.setRegions(regions);

    grid->setPressure(P);
    grid->setPermx(kx);
    grid->setPermy(ky);
    grid->setPermz(kz);
    grid->setDx(dx);
    grid->setDy(dy);
    grid->setDz(dz);
    grid->setPoro(poro);

    grid->setPVT(pvt);
    grid->setRock(rock);
    grid->init();

    well = new Well();

    Eigen::Matrix<double, Eigen::Dynamic, 4> welltrack;
    welltrack.resize(2, 4);

    welltrack <<
    50, 50, 0 , 0,
    50, 50, 10, 10;

    WellData* welldata = new WellData();
    Control control {UnitType::PRESSURE,0,10E+5};
    Perforation perforation {false,0,100*86400,0,5,0.074,0,1.0};

    welldata->sePerforation({perforation});
    welldata->setControls({control});
    welldata->setTrajectory(welltrack);


    well->setGrid(grid);
    well->setData(welldata);

    well->init();
    */
}


void GridDataWidget::on_btn_import_grid_clicked() {
    processBlockCenteredGrid(); // Блочно-центрированная геометрия
    //processCornerPointGrid();   // Геометрия угловой точки
    //processVertexDefinedGrid(); // Задание сетки вершинами блоков
}


void GridDataWidget::processBlockCenteredGrid() {

}


void GridDataWidget::processCornerPointGrid() {

}


void GridDataWidget::processVertexDefinedGrid() {

}
