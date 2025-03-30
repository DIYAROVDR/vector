#include "h5filemanager.h"

H5FileManager& H5FileManager::instance() {
    static H5FileManager h5filemanager;
    return h5filemanager;
}


bool H5FileManager::isOpen() {
    return openFlag;
}


H5FileManager::H5FileManager() {
    datatypes = {
        {AttributeTypes::YEAR, "year"},
        {AttributeTypes::MONTH, "month"},
        {AttributeTypes::DAY, "day"},
        {AttributeTypes::HOUR, "hour"},
        {AttributeTypes::MINUTE, "minute"},
        {AttributeTypes::SECOND, "second"},
        {AttributeTypes::UNIT_SYSTEM_TYPE, "unit_system_type"},
        {AttributeTypes::WATER, "water"},
        {AttributeTypes::OIL, "oil"},
        {AttributeTypes::GAS, "gas"},
        {AttributeTypes::DISGAS, "disgas"},
        {AttributeTypes::VAPOIL, "vapoil"},
        {AttributeTypes::FIPNUM, "fipnum"},
        {AttributeTypes::PVTNUM, "pvtnum"},
        {AttributeTypes::SATNUM, "satnum"},
        {AttributeTypes::ROCKNUM, "rocknum"},
        {AttributeTypes::EQLNUM, "eqlnum"},
        {AttributeTypes::NX, "nx"},
        {AttributeTypes::NY, "ny"},
        {AttributeTypes::NZ, "nz"},
        {AttributeTypes::GRID_TYPE, "type_grid"},
        {AttributeTypes::LAYERS_COUNT, "layers_count"},
        {AttributeTypes::PINCH_VERT, "pinch_vert"},
        {AttributeTypes::PINCH_HOR, "pinch_hor"}
    };
}


H5FileManager::~H5FileManager() {}


void H5FileManager::createAttribute(const std::map<std::string, std::variant<int, double>>& data, H5::Group& group) {
    H5::DataSpace attr_space(H5S_SCALAR);
    for (const auto& pair : data) {
        if (std::holds_alternative<int>(pair.second)) {
            H5::Attribute attr = group.createAttribute(pair.first, H5::PredType::NATIVE_INT, attr_space);
            attr.write(H5::PredType::NATIVE_INT, &std::get<int>(pair.second));
        } else if (std::holds_alternative<double>(pair.second)) {
            H5::Attribute attr = group.createAttribute(pair.first, H5::PredType::NATIVE_DOUBLE, attr_space);
            attr.write(H5::PredType::NATIVE_DOUBLE, &std::get<double>(pair.second));
        }
    }
}


void H5FileManager::saveToAttribute(const std::map<std::string, std::variant<int, double>>& data, H5::Group& group) {
    H5::DataSpace attr_space(H5S_SCALAR);

    for (const auto& pair : data) {
        if (std::holds_alternative<int>(pair.second)) {
            H5::Attribute attr = group.openAttribute(pair.first);
            attr.write(H5::PredType::NATIVE_INT, &std::get<int>(pair.second));
        } else if (std::holds_alternative<double>(pair.second)) {
            H5::Attribute attr = group.openAttribute(pair.first);
            attr.write(H5::PredType::NATIVE_DOUBLE, &std::get<double>(pair.second));
        }
    }

}


void H5FileManager::saveStringAttribute(H5::Group &group, const std::string &name, const std::string &value) {
    // Общий метод для сохранения строкового атрибута
    H5::DataSpace attr_space(H5S_SCALAR);
    H5::StrType str_type(H5::PredType::C_S1, value.size());
    if (group.attrExists(name)) {
        group.removeAttr(name);
    }
    H5::Attribute attr = group.createAttribute(name, str_type, attr_space);
    attr.write(str_type, value);
}

std::string H5FileManager::readStringAttribute(H5::Group &group, const std::string &name) {
    // Общий метод для чтения строкового атрибута
    if (!openFlag || !group.attrExists(name)) {
        return "";
    }
    H5::Attribute attr = group.openAttribute(name);
    H5::DataType dtype = attr.getDataType();
    size_t size = dtype.getSize();
    std::string value(size, '\0');
    attr.read(dtype, &value[0]);
    value.resize(strlen(value.c_str()));
    return value;
}

H5::Group H5FileManager::getWellGroup(const std::string& name) {
    if (!wells.exists(name)) {
        return wells.createGroup(name);
    }
    return wells.openGroup(name);
}


void H5FileManager::openFile(const std::string& path) {
    currentPath = path;
    if (std::filesystem::exists(currentPath)) {
        file = H5::H5File(currentPath, H5F_ACC_RDWR);
    } else {
        file = H5::H5File(currentPath, H5F_ACC_TRUNC);
        general = file.createGroup("/general");
        grid = file.createGroup("/grid");
        pvt = file.createGroup("/pvt");
        rock = file.createGroup("/rock");
        static_cube = file.createGroup("/static_cube");
        dynamic_cube = file.createGroup("/dynamic_cube");
        wells = file.createGroup("/wells");

        H5::DataSpace attr_space(H5S_SCALAR);

        int initial_step = 0;
        H5::Attribute step_attr = dynamic_cube.createAttribute("current_step", H5::PredType::NATIVE_INT, attr_space);
        step_attr.write(H5::PredType::NATIVE_INT, &initial_step);

        datageneral = {
            {"year", 2001},
            {"month", 6},
            {"day", 10},
            {"hour", 0},
            {"minute", 0},
            {"second", 0},
            {"unit_system_type", static_cast<int>(Unit::System::TS)},
            {"water", 1},
            {"oil", 0},
            {"gas", 0},
            {"disgas", 0},
            {"vapoil", 1},
            {"fipnum",1},
            {"pvtnum",1},
            {"satnum",1},
            {"rocknum", 1},
            {"eqlnum", 1}
        };

        createAttribute(datageneral, general);

        datagrid = {
            {"nx",1},
            {"ny",1},
            {"nz",1},
            {"type_grid", static_cast<int>(Grid::Type::BLOCKCENTERED)},
            {"layers_count", 1},
            {"pinch_vert", 0.0},
            {"pinch_hor", 0.0}
        };

        createAttribute(datagrid, grid);
    }
    openFlag = true;
}


void H5FileManager::setDimens(int nx, int ny, int nz) {
    H5::DataSpace attr_space(H5S_SCALAR);

    H5::Attribute nx_attr = grid.openAttribute("nx");
    nx_attr.write(H5::PredType::NATIVE_INT, &nx);

    H5::Attribute ny_attr = grid.openAttribute("ny");
    ny_attr.write(H5::PredType::NATIVE_INT, &ny);

    H5::Attribute nz_attr = grid.openAttribute("nz");
    nz_attr.write(H5::PredType::NATIVE_INT, &nz);
}


void H5FileManager::saveStaticCube(const Eigen::ArrayXd& cube, const std::string& name) {
    saveDataWithCheck(static_cube, name, cube);
}


void H5FileManager::setStartDate(const std::array<int,6>& date) {
    size_t item = 0;
    for (const auto& pair : datatypes) {
        if (pair.first > AttributeTypes::DATES && pair.first < AttributeTypes::UNIT) {
            datageneral[pair.second] = date[item];
            ++item;
        }
    }
    saveToAttribute(datageneral,general);
}


void H5FileManager::setUnitSystem(Unit::System system) {
    saveScalarAttribute(general, "unit_system", static_cast<int>(system));
}


void H5FileManager::setTypeInitialization(Grid::Initialization type) {
    saveScalarAttribute(general, "type_initialization", static_cast<int>(type));
}


void H5FileManager::setFluids(const std::array<int,5>& fluids) {
    size_t item = 0;
    for (const auto& pair : datatypes) {
        if (pair.first >= AttributeTypes::WATER && pair.first <= AttributeTypes::VAPOIL) {
            datageneral[pair.second] = fluids[item];
            ++item;
        }
    }
    saveToAttribute(datageneral,general);
}


void H5FileManager::setRegions(const std::array<int,5>& reg) {
    size_t item = 0;
    for (const auto& pair : datatypes) {
        if (pair.first >= AttributeTypes::FIPNUM && pair.first <= AttributeTypes::EQLNUM) {
            datageneral[pair.second] = reg[item];
            ++item;
        }
    }
    saveToAttribute(datageneral,general);
}


void H5FileManager::setTypeGrid(Grid::Type type) {
    H5::DataSpace attr_space(H5S_SCALAR);
    int type_value = static_cast<int>(type);

    if(general.attrExists("type_grid")){
        general.removeAttr("type_grid");
    }

    H5::Attribute system_attr = general.createAttribute("type_grid", H5::PredType::NATIVE_INT, attr_space);
    system_attr.write(H5::PredType::NATIVE_INT,&type_value);
}


void H5FileManager::saveDynamicCube(const Eigen::ArrayXd& cube, const std::string& name) {
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

    std::string array_name = name + "_" + std::to_string(current_step);

    // Сохраняем массив в подгруппу
    EigenHDF5::save(cube_group, array_name, cube);

    // Увеличиваем шаг и обновляем атрибут current_step
    ++current_step;
    step_attr.write(H5::PredType::NATIVE_INT, &current_step);
}


Eigen::ArrayXd H5FileManager::getDynamicCube(const std::string &name, int step) {
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


void H5FileManager::setCoord(const Eigen::Matrix<double, Eigen::Dynamic, 6>& coord) {
    EigenHDF5::save(grid,"coord",coord);
}


void H5FileManager::setZcorn(const Eigen::Matrix<double, Eigen::Dynamic, 6>& zcorn) {
    EigenHDF5::save(grid,"zcorn",zcorn);
}


void H5FileManager::setHexahedronVertexOrder(const Eigen::VectorXi& hexahedrons) {
    saveDataWithCheck(grid, "hexahedrons", hexahedrons);
}


std::string H5FileManager::startDate() {
    return readStringAttribute(general, "start_date");
}


Unit::System H5FileManager::unitSystem() {
    return static_cast<Unit::System>(readScalarAttribute(general, "unit_system", static_cast<int>(Unit::System::TS)));
}


Grid::Initialization H5FileManager::typeInitialization() {
    auto defval = static_cast<int>(Grid::Initialization::EQUILIBRIUM);
    return static_cast<Grid::Initialization>(readScalarAttribute(general, "type_initialization", defval));
}


Grid::Type H5FileManager::typeGrid() {
    auto defval = static_cast<int>(Grid::Type::BLOCKCENTERED);
    return static_cast<Grid::Type>(readScalarAttribute(general, "type_grid", defval));
}


Eigen::Array<int, 5, 1> H5FileManager::regions() {
    Eigen::Array<int, 5, 1> reg;

    return reg;
}


Eigen::Array3i H5FileManager::dimens() {
    Eigen::Array3i dim{1,1,1};

    if (!openFlag) {
        return dim;
    }

    H5::Attribute nx_attr = grid.openAttribute("nx");
    nx_attr.read(H5::PredType::NATIVE_INT, &dim[0]);

    H5::Attribute ny_attr = grid.openAttribute("ny");
    ny_attr.read(H5::PredType::NATIVE_INT, &dim[1]);

    H5::Attribute nz_attr = grid.openAttribute("nz");
    nz_attr.read(H5::PredType::NATIVE_INT, &dim[2]);

    return dim;
}


Eigen::Matrix<double, Eigen::Dynamic, 3> H5FileManager::loadControls(const std::string& name) {
    return loadWellData<double, 3>(name, "controls");
}


void H5FileManager::saveControls(const std::string& name, const Eigen::Matrix<double, Eigen::Dynamic, 3>& controls) {
    H5::Group wellGroup = getWellGroup(name);
    saveDataWithCheck(wellGroup, "controls", controls);
}


Eigen::Matrix<double, Eigen::Dynamic, 7> H5FileManager::loadPerforation(const std::string& name) {
    return loadWellData<double, 7>(name, "perforation");
}


void H5FileManager::savePerforation(const std::string &name, const Eigen::Matrix<double, Eigen::Dynamic, 7>& perforation) {
    H5::Group wellGroup = getWellGroup(name);
    saveDataWithCheck(wellGroup, "perforation", perforation);
}


Eigen::Matrix<double, Eigen::Dynamic, 4> H5FileManager::loadWelltrack(const std::string &name) {
    return loadWellData<double, 4>(name, "welltrack");
}


void H5FileManager::saveWellTrack(const std::string& name, const Eigen::Matrix<double, Eigen::Dynamic, 4>& welltrack) {
    H5::Group wellGroup = getWellGroup(name);
    saveDataWithCheck(wellGroup, "welltrack", welltrack);
}


Eigen::ArrayXd H5FileManager::getStaticCube(const std::string &name) {
    Eigen::ArrayXd cube;
    EigenHDF5::load(static_cube,name,cube);
    return cube;
}







