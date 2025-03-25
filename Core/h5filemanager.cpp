#include "h5filemanager.h"


H5FileManager& H5FileManager::instance() {
    static H5FileManager h5filemanager;
    return h5filemanager;
}

bool H5FileManager::isOpen() {
    return openFlag;
}


H5FileManager::H5FileManager(){}


H5FileManager::~H5FileManager() {}


void H5FileManager::openFile(const std::string& path) {
    currentPath = path;
    if (std::filesystem::exists(currentPath)) {
        file = H5::H5File(currentPath, H5F_ACC_RDWR);
    } else {
        file = H5::H5File(currentPath, H5F_ACC_TRUNC);
        model = file.createGroup("/model");
        grid = file.createGroup("/model/grid");
        static_cube = file.createGroup("/static_cube");
        dynamic_cube = file.createGroup("/dynamic_cube");
        wells = file.createGroup("/wells");

        int initial_step = 0;
        H5::DataSpace attr_space(H5S_SCALAR);
        H5::Attribute step_attr = dynamic_cube.createAttribute("current_step", H5::PredType::NATIVE_INT, attr_space);
        step_attr.write(H5::PredType::NATIVE_INT, &initial_step);
    }
    openFlag = true;
}


void H5FileManager::setDimens(int nx, int ny, int nz) {
    this->nx = nx;
    this->ny = ny;
    this->nz = nz;

    if (grid.exists("dimensions")) {
        grid.unlink("dimensions");
    }
    EigenHDF5::save(grid, "dimensions", Eigen::Vector3i(nx, ny, nz));
}


void H5FileManager::saveStaticCube(const Eigen::VectorXd &cube, const std::string &name) {
    if(static_cube.exists(name)) {
        static_cube.unlink(name);
    }
    EigenHDF5::save(static_cube, name, cube);
}


void H5FileManager::setStartDate(const std::string& date) {
    H5::DataSpace attr_space(H5S_SCALAR);
    H5::StrType str_type(H5::PredType::C_S1, date.size());

    // Если атрибут уже существует, удаляем его
    if (model.attrExists("start_date")) {
        model.removeAttr("start_date");
    }

    // Создаем новый атрибут
    H5::Attribute date_attr = model.createAttribute("start_date", str_type, attr_space);
    date_attr.write(str_type, date);
}


void H5FileManager::setUnitSystem(Unit::System system) {
    H5::DataSpace attr_space(H5S_SCALAR);
    int system_value = static_cast<int>(system);

    if(model.attrExists("unit_system")){
         model.removeAttr("unit_system");
    }

    H5::Attribute system_attr = model.createAttribute("unit_system", H5::PredType::NATIVE_INT, attr_space);
    system_attr.write(H5::PredType::NATIVE_INT, &system_value);
}


void H5FileManager::setFluids(const std::vector<bool>& fluids) {
    // Создаем пространство данных для массива
    hsize_t dims[1] = {fluids.size()};
    H5::DataSpace dataspace(1, dims);

    // Преобразуем enum в массив целых чисел
    std::vector<int> fluid_values;
    for (const auto& fluid : fluids) {
        fluid_values.push_back(static_cast<int>(fluid));
    }

    // Создаем набор данных для хранения флюидов
    H5::DataSet dataset = model.createDataSet("fluids", H5::PredType::NATIVE_INT, dataspace);
    dataset.write(fluid_values.data(), H5::PredType::NATIVE_INT);
}


void H5FileManager::setTypeInitialization(Grid::Initialization type) {
    H5::DataSpace attr_space(H5S_SCALAR); // Скалярное пространство данных
    int type_value = static_cast<int>(type); // Преобразуем enum в int

    if(model.attrExists("type_initialization")){
        model.removeAttr("type_initialization");
    }

    H5::Attribute system_attr = model.createAttribute("type_initialization", H5::PredType::NATIVE_INT, attr_space);
    system_attr.write(H5::PredType::NATIVE_INT,&type_value);
}


void H5FileManager::setRegions(const std::vector<int>& regions) {
    hsize_t dims[1] = {regions.size()};
    H5::DataSpace dataspace(1, dims);

    if (model.exists("regions")) {
        model.unlink("regions");
    }

    H5::DataSet dataset = model.createDataSet("regions", H5::PredType::NATIVE_INT, dataspace);
    dataset.write(regions.data(), H5::PredType::NATIVE_INT);
}

void H5FileManager::setTypeGrid(Grid::Type type) {
    H5::DataSpace attr_space(H5S_SCALAR); // Скалярное пространство данных
    int type_value = static_cast<int>(type); // Преобразуем enum в int

    if(model.attrExists("type_grid")){
        model.removeAttr("type_grid");
    }

    H5::Attribute system_attr = model.createAttribute("type_grid", H5::PredType::NATIVE_INT, attr_space);
    system_attr.write(H5::PredType::NATIVE_INT,&type_value);
}


void H5FileManager::saveCube(const Eigen::ArrayXd& cube, const std::string& name) {
    // Получаем текущий шаг из атрибута current_step
    int current_step = 0;
    H5::Attribute step_attr = dynamic_cube.openAttribute("current_step");
    step_attr.read(H5::PredType::NATIVE_INT, &current_step);

    // Создаём или открываем подгруппу для данного массива (cubename)
    H5::Group cube_group;
    if (!dynamic_cube.exists(name)) {
        cube_group = dynamic_cube.createGroup(name);
    } else {
        cube_group = dynamic_cube.openGroup(name);
    }

    // Создаём имя для нового массива с текущим шагом
    std::string array_name = name + "_" + std::to_string(current_step);
    std::cout<<array_name<<std::endl;

    // Сохраняем массив в подгруппу
    EigenHDF5::save(cube_group, array_name, cube);

    // Увеличиваем шаг и обновляем атрибут current_step
    ++current_step;
    step_attr.write(H5::PredType::NATIVE_INT, &current_step);
}

Eigen::ArrayXd H5FileManager::getCube(const std::string &name, int step) {
    // Формируем полное имя массива с учётом шага
    std::string array_name = name + "_" + std::to_string(step);

    // Открываем группу dynamic_cube
    H5::Group dynamic_cube = file.openGroup("/dynamic_cube");

    // Открываем подгруппу
    H5::Group cube_group = dynamic_cube.openGroup(name);

    // Читаем массив из файла
    Eigen::ArrayXd cube;
    EigenHDF5::load(cube_group, array_name, cube);

    return cube;
}


void H5FileManager::setNodes(const Eigen::Matrix<double, Eigen::Dynamic, 3>& nodes) {
    EigenHDF5::save(grid,"nodes",nodes);
}


void H5FileManager::setCoord(const Eigen::Matrix<double, Eigen::Dynamic, 6> &coord) {
    EigenHDF5::save(grid,"coord",coord);
}


void H5FileManager::setZcorn(const Eigen::Matrix<double, Eigen::Dynamic, 6> &zcorn) {
    EigenHDF5::save(grid,"zcorn",zcorn);
}


void H5FileManager::setHexahedronVertexOrder(const Eigen::VectorXi &hexahedrons) {
    if(static_cube.exists("hexahedrons")) {
        static_cube.unlink("hexahedrons");
    }
    EigenHDF5::save(grid, "hexahedrons", hexahedrons);
}


std::string H5FileManager::startDate() {
    if (!openFlag) {
        return std::string();
    }

    if (!model.attrExists("start_date")) {
        return std::string();
    }

    H5::Attribute date_attr = model.openAttribute("start_date");
    H5::DataType dtype = date_attr.getDataType();
    H5::DataSpace dspace = date_attr.getSpace();

    size_t size = dtype.getSize();
    std::string date(size, '\0');
    date_attr.read(dtype, &date[0]);
    date.resize(strlen(date.c_str()));

    return date;
}


Unit::System H5FileManager::unitSystem() {
    if (!openFlag) {
        return Unit::System::TS;
    }


    if (!model.attrExists("unit_system")) {
        return Unit::System::TS;
    }

    H5::Attribute system_attr = model.openAttribute("unit_system");

    int system_value;
    system_attr.read(H5::PredType::NATIVE_INT, &system_value);

    return static_cast<Unit::System>(system_value);
}


Grid::Initialization H5FileManager::typeInitialization() {
    if (!openFlag) {
        return Grid::Initialization::EQUILIBRIUM;
    }


    if (!model.attrExists("type_initialization")) {
        return Grid::Initialization::EQUILIBRIUM;
    }

    H5::Attribute system_attr = model.openAttribute("type_initialization");

    int system_value;
    system_attr.read(H5::PredType::NATIVE_INT, &system_value);

    return static_cast<Grid::Initialization>(system_value);
}

Grid::Type H5FileManager::typeGrid() {
    if (!openFlag) {
        return Grid::Type::BLOCKCENTERED;
    }


    if (!model.attrExists("type_grid")) {
        return Grid::Type::BLOCKCENTERED;
    }

    H5::Attribute system_attr = model.openAttribute("type_grid");

    int system_value;
    system_attr.read(H5::PredType::NATIVE_INT, &system_value);

    return static_cast<Grid::Type>(system_value);
}


std::vector<int> H5FileManager::regions() {
    std::vector<int> regions(5,1);

    if (!openFlag) {
        return regions;
    }

    // Проверяем, существует ли набор данных "regions" в файле
    if (!model.exists("regions")) {
        // Если набор данных не существует, возвращаем пустой вектор
        return regions;
    }

    // Открываем набор данных "regions"
    H5::DataSet dataset = model.openDataSet("regions");

    // Получаем пространство данных набора данных
    H5::DataSpace dataspace = dataset.getSpace();

    // Получаем размерность данных
    hsize_t dims[1];
    dataspace.getSimpleExtentDims(dims, nullptr);

    // Изменяем размер вектора для хранения данных
    regions.resize(dims[0]);

    // Читаем данные из набора данных в вектор
    dataset.read(regions.data(), H5::PredType::NATIVE_INT);

    return regions;
}


Eigen::Array3i H5FileManager::dimens() {
    Eigen::Array3i dim{1,1,1};

    if (!openFlag) {
        return dim;
    }

    // Проверяем, существует ли набор данных "regions" в файле
    if (!grid.exists("dimensions")) {
        return dim;
    }

    EigenHDF5::load(grid,"dimensions",dim);
    return dim;
}


Eigen::Matrix<double, Eigen::Dynamic, 3> H5FileManager::loadControls(const std::string& name) {
    Eigen::Matrix<double, Eigen::Dynamic, 3> controls;

    if (wells.exists(name)) {
        H5::Group wellGroup = wells.openGroup(name);
        if (wellGroup.exists("controls")) {
            EigenHDF5::load(wellGroup, "controls", controls);
        }
    }

    return controls;
}


void H5FileManager::saveControls(const std::string& name, const Eigen::Matrix<double, Eigen::Dynamic, 3>& controls) {
    H5::Group wellGroup;
    if (!wells.exists(name)) {
        wellGroup = wells.createGroup(name);
    } else {
        wellGroup = wells.openGroup(name);
    }

    if (wellGroup.exists("controls")) {
        wellGroup.unlink("controls");
    }

    EigenHDF5::save(wellGroup, "controls", controls);
}


Eigen::Matrix<double, Eigen::Dynamic, 7> H5FileManager::loadPerforation(const std::string &name) {
    Eigen::Matrix<double, Eigen::Dynamic, 7> perforation;

    if (wells.exists(name)) {
        H5::Group wellGroup = wells.openGroup(name);
        if (wellGroup.exists("perforation")) {
            EigenHDF5::load(wellGroup, "perforation", perforation);
        }
    }

    return perforation;
}


void H5FileManager::savePerforation(const std::string &name, const Eigen::Matrix<double, Eigen::Dynamic, 7>& perforation) {
    H5::Group wellGroup;
    if (!wells.exists(name)) {
        wellGroup = wells.createGroup(name);
    } else {
        wellGroup = wells.openGroup(name);
    }

    if (wellGroup.exists("perforation")) {
        wellGroup.unlink("perforation");
    }

    EigenHDF5::save(wellGroup, "perforation", perforation);
}


Eigen::Matrix<double, Eigen::Dynamic, 4> H5FileManager::loadWelltrack(const std::string &name) {
    Eigen::Matrix<double, Eigen::Dynamic, 4> welltrack;
    if (wells.exists(name)) {
        H5::Group wellGroup = wells.openGroup(name);
        if (wellGroup.exists("welltrack")) {
            EigenHDF5::load(wellGroup, "welltrack", welltrack);
        }
    }

    return welltrack;
}


void H5FileManager::saveWellTrack(const std::string &name, const Eigen::Matrix<double, Eigen::Dynamic, 4> &welltrack) {
    H5::Group wellGroup;
    if (!wells.exists(name)) {
        wellGroup = wells.createGroup(name);
    } else {
        wellGroup = wells.openGroup(name);
    }

    if (wellGroup.exists("welltrack")) {
        wellGroup.unlink("welltrack");
    }
    EigenHDF5::save(wellGroup, "welltrack", welltrack);
}


Eigen::VectorXd H5FileManager::getStaticCube(const std::string &name) {
    Eigen::VectorXd cube;
    EigenHDF5::load(static_cube,name,cube);
    return cube;
}


vtkSmartPointer<vtkDoubleArray> H5FileManager::loadScalar(size_t step) {
    auto scalarData = getCube("PRESSURE",step);
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




