#ifndef H5FILEMANAGER_H
#define H5FILEMANAGER_H

#include <string>
#include <variant>
#include <filesystem>
#include <Eigen/Core>

#include "grid.h"
#include "eigen3-hdf5.hpp"
#include "physicalquantity.h"

class H5FileManager {
public:
    void openFile(const std::string& path);
    void setDimens(int nx, int ny, int nz);

    void saveStaticCube(const Eigen::ArrayXd& cube,const std::string& name);
    void setStartDate(const std::array<int,6>& date);
    void setFluids(const std::array<int,5>& fluids);
    void setRegions(const std::array<int,5>& regions);
    void setUnitSystem(Unit::System type);
    void setTypeInitialization(Grid::Initialization type);
    void setTypeGrid(Grid::Type type);

    void saveDynamicCube(const Eigen::ArrayXd& cube,const std::string& name);
    void setNodes(const Eigen::Matrix<double, Eigen::Dynamic, 3>& nodes);
    void setCoord(const Eigen::Matrix<double, Eigen::Dynamic, 6>& coords);
    void setZcorn(const Eigen::Matrix<double, Eigen::Dynamic, 6>& zcorn);
    void setHexahedronVertexOrder(const Eigen::VectorXi& hexahedrons);
    void saveWellTrack(const std::string& name, const Eigen::Matrix<double, Eigen::Dynamic, 4>& welltrack);
    void savePerforation(const std::string& name, const Eigen::Matrix<double, Eigen::Dynamic, 7>& perforation);
    void saveControls(const std::string& name, const Eigen::Matrix<double, Eigen::Dynamic, 3>& controls);

    std::string startDate();
    Unit::System unitSystem();
    Grid::Initialization typeInitialization();
    Grid::Type typeGrid();
    Eigen::Array<int, 5, 1> regions();
    Eigen::Array3i dimens();

    Eigen::Matrix<double, Eigen::Dynamic, 4> loadWelltrack(const std::string& name);    
    Eigen::Matrix<double, Eigen::Dynamic, 7> loadPerforation(const std::string& name);
    Eigen::Matrix<double, Eigen::Dynamic, 3> loadControls(const std::string& name);
    Eigen::ArrayXd getDynamicCube(const std::string& name, int step);
    Eigen::ArrayXd getStaticCube(const std::string& name);

    static H5FileManager& instance();
    bool isOpen();

private:
    H5FileManager();
    ~H5FileManager();
    H5FileManager(const H5FileManager&) = delete;
    H5FileManager& operator =(const H5FileManager&) = delete;
    bool openFlag = false;
    std::string currentPath;
    H5::H5File file;
    H5::Group general;
    H5::Group grid;
    H5::Group pvt;
    H5::Group rock;
    H5::Group static_cube;
    H5::Group dynamic_cube;
    H5::Group wells;

    enum class AttributeTypes {
        DATES,
        YEAR,
        MONTH,
        DAY,
        HOUR,
        MINUTE,
        SECOND,
        UNIT,
        UNIT_SYSTEM_TYPE,
        FLUIDS,
        WATER,
        OIL,
        GAS,
        DISGAS,
        VAPOIL,
        REGIONS,
        FIPNUM,
        PVTNUM,
        SATNUM,
        ROCKNUM,
        EQLNUM,
        PVT,
        GRID,
        NX,
        NY,
        NZ,
        GRID_TYPE,
        LAYERS_COUNT,
        PINCH_VERT,
        PINCH_HOR
    };

    std::map<AttributeTypes, std::string> datatypes;
    std::map<std::string, std::variant<int, double>> datageneral;
    std::map<std::string, std::variant<int, double>> datagrid;

    void createAttribute(const std::map<std::string, std::variant<int, double>>& data, H5::Group& group);
    void saveToAttribute(const std::map<std::string, std::variant<int, double>>& data, H5::Group& group);
    void saveStringAttribute(H5::Group& group, const std::string& name, const std::string& value);
    H5::Group getWellGroup(const std::string& name);
    std::string readStringAttribute(H5::Group& group, const std::string& name);


    // Общий метод для сохранения скалярного атрибута
    template<typename T>
    void saveScalarAttribute(H5::Group& group, const std::string& name, const T& value) {
        H5::DataSpace attr_space(H5S_SCALAR);
        if (group.attrExists(name)) {
            group.removeAttr(name);
        }
        H5::Attribute attr = group.createAttribute(name, H5::PredType::NATIVE_INT, attr_space);
        attr.write(H5::PredType::NATIVE_INT, &value);
    }


    // Общий метод для чтения скалярного атрибута
    template<typename T>
    T readScalarAttribute(H5::Group& group, const std::string& name, T defaultValue) {
        if (!openFlag || !group.attrExists(name)) {
            return defaultValue;
        }
        T value;
        H5::Attribute attr = group.openAttribute(name);
        attr.read(H5::PredType::NATIVE_INT, &value);
        return value;
    }


    // Общий метод для сохранения данных в группе с проверкой существования
    template<typename T>
    void saveDataWithCheck(H5::Group& group, const std::string& name, const T& data) {
        if (group.exists(name)) {
            group.unlink(name);
        }
        EigenHDF5::save(group, name, data);
    }


    template<typename T, int Cols>
    Eigen::Matrix<double, Eigen::Dynamic, Cols> loadWellData(const std::string& wellName, const std::string& dataName) {
        Eigen::Matrix<double, Eigen::Dynamic, Cols> data;
        if (wells.exists(wellName)) {
            H5::Group wellGroup = wells.openGroup(wellName);
            if (wellGroup.exists(dataName)) {
                EigenHDF5::load(wellGroup, dataName, data);
            }
        }
        return data;
    }
};

#endif // H5FILEMANAGER_H
