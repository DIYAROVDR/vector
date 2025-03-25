#ifndef H5FILEMANAGER_H
#define H5FILEMANAGER_H

#include <cstdint>
#include <string>
#include <filesystem>

#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkDoubleArray.h>
#include <vtkIntArray.h>
#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>
#include <vtkHexahedron.h>

#include <vtkHDFWriter.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>

#include <Eigen/Dense>
#include <Eigen/Core>
#include <Eigen/Sparse>

#include "grid.h"
#include "eigen3-hdf5.hpp"
#include "physicalquantity.h"
#include "pvt.h"

class H5FileManager {
public:
    void openFile(const std::string& path);
    void setDimens(int nx, int ny, int nz);

    void saveStaticCube(const Eigen::VectorXd& cube,const std::string& name);
    void setStartDate(const std::string& date);
    void setUnitSystem(Unit::System type);
    void setFluids(const std::vector<bool>& fluids);
    void setTypeInitialization(Grid::Initialization type);
    void setRegions(const std::vector<int>& regions);
    void setTypeGrid(Grid::Type type);

    void saveCube(const Eigen::ArrayXd& cube,const std::string& name);
    void setNodes(const Eigen::Matrix<double, Eigen::Dynamic, 3>& nodes);
    void setCoord(const Eigen::Matrix<double, Eigen::Dynamic, 6>& coords);
    void setZcorn(const Eigen::Matrix<double, Eigen::Dynamic, 6>& zcorn);
    void setHexahedronVertexOrder(const Eigen::VectorXi& hexahedrons);
    void saveWellTrack(const Eigen::Matrix<double, Eigen::Dynamic, 4>& zcorn);
    void saveWellTrack(const std::string& name, const Eigen::Matrix<double, Eigen::Dynamic, 4>& welltrack);
    void savePerforation(const std::string& name, const Eigen::Matrix<double, Eigen::Dynamic, 7>& perforation);
    void saveControls(const std::string& name, const Eigen::Matrix<double, Eigen::Dynamic, 3>& controls);

    std::string startDate();
    Unit::System unitSystem();
    Grid::Initialization typeInitialization();
    Grid::Type typeGrid();
    std::vector<int> regions();
    Eigen::Array3i dimens();

    Eigen::Matrix<double, Eigen::Dynamic, 4> loadWelltrack(const std::string& name);    
    Eigen::Matrix<double, Eigen::Dynamic, 7> loadPerforation(const std::string& name);
    Eigen::Matrix<double, Eigen::Dynamic, 3> loadControls(const std::string& name);
    Eigen::ArrayXd getCube(const std::string& name, int step);
    Eigen::VectorXd getStaticCube(const std::string& name);    

    vtkSmartPointer<vtkDoubleArray> loadScalar(size_t step = 0);
    vtkSmartPointer<vtkStructuredGrid> loadGrid();
    vtkSmartPointer<vtkUnstructuredGrid> loadGridCP();

    static H5FileManager& instance();
    bool isOpen();
private:
    H5FileManager();
    ~H5FileManager();
    H5FileManager(const H5FileManager&) = delete;
    H5FileManager& operator =(const H5FileManager&) = delete;
private:
    bool openFlag = false;
    int nx;
    int ny;
    int nz;
    std::string currentPath;
    H5::H5File file;
    H5::Group model;
    H5::Group grid;
    H5::Group static_cube;
    H5::Group dynamic_cube;
    H5::Group wells;
};

#endif // H5FILEMANAGER_H
