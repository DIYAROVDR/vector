cmake_minimum_required(VERSION 3.16)
project(vector VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_VERSION_MAJOR 5)

if(UNIX)
    set(Eigen3_INCLUDE_DIR "/home/diyarovdr/lib/eigen-3.4.0")
    set(Eigen3_HDF5_INCLUDE_DIR "/home/diyarovdr/lib/eigen-hdf5")
    set(HDF5_ROOT "/usr/local/HDF_Group/HDF5/1.14.2")
    set(Python3_ROOT_DIR "/usr/bin/python3.10")
    set(Python3_EXECUTABLE "/usr/bin/python3.10")
    set(Python3_INCLUDE_DIR "/usr/include/python3.10")
    set(Python3_LIBRARY "/usr/lib/x86_64-linux-gnu/libpython3.10.so")
elseif(WIN32)
    set(Eigen3_INCLUDE_DIR "c:/Users/Damir/Desktop/main/lib/eigen-3.4.0")
    set(Python3_ROOT_DIR "c:/Program Files/Python310")
    set(Python3_EXECUTABLE "c:/Program Files/Python310/python.exe")
    set(Python3_INCLUDE_DIR "c:/Program Files/Python310/include")
    set(Python3_LIBRARY "c:/Program Files/Python310/libs/python310.lib")
endif()


find_package(Qt5 REQUIRED COMPONENTS
    Widgets
    OpenGL
    Charts
)

find_package(HDF5 REQUIRED COMPONENTS
    C
    CXX
)

find_package(VTK REQUIRED COMPONENTS
    CommonColor
    CommonCore
    CommonDataModel
    FiltersGeometry
    FiltersSources
    InteractionStyle
    RenderingContextOpenGL2
    RenderingCore
    RenderingFreeType
    RenderingGL2PSOpenGL2
    RenderingOpenGL2
    GUISupportQt
    RenderingAnnotation
    RenderingVolume
    WrappingPythonCore
)

set(CORE_CPP_SOURCES
    Core/h5filemanager.cpp
    Core/physicalquantity.cpp
    Core/unit.cpp
    Core/blockcenteredgrid.cpp
    Core/solver.cpp
    Core/grid.cpp
    Core/pvtw.cpp
    Core/pvt.cpp
    Core/pvcdo.cpp
    Core/well.cpp
    Core/pvdo.cpp
    Core/rock.cpp
    Core/welldata.cpp
    Core/density.cpp
    Core/rockdata.cpp
    Core/pvtbase.cpp
    Core/wells.cpp
    Core/pvdg.cpp
)

set(GUI_CPP_SOURCES
    GUI/spatialviewwidget.cpp
    GUI/pvttreemodel.cpp
    GUI/generalmodel.cpp
    GUI/cubestreemodel.cpp
    GUI/wellstreewidget.cpp
    GUI/griddatamodel.cpp
    GUI/fillcubewidget.cpp
    GUI/projectdatawidget.cpp
    GUI/mainwindow.cpp
    GUI/homewidget.cpp
    GUI/wellsdatawidget.cpp
    GUI/perforationmodel.cpp
    GUI/cubetreewidget.cpp
    GUI/multidelegate.cpp
    GUI/griddatawidget.cpp
    GUI/wellcontrolmodel.cpp
    GUI/layerfillcubemodel.cpp
    GUI/codeeditor.cpp
    GUI/addviewwidget.cpp
    GUI/multiviewidget.cpp
    GUI/pvtchartmodel.cpp
    GUI/welltrajectorymodel.cpp
    GUI/timesliderwidget.cpp
    GUI/pvttablemodel.cpp
    GUI/scriptsyntaxhighlighter.cpp
    GUI/selectcubewidget.cpp
    GUI/chartview.cpp
    GUI/aspectratiopixmaplabel.cpp
    GUI/pvtdatawidget.cpp
    GUI/scientificspinbox.cpp
    GUI/rockdatawidget.cpp
    GUI/gridviewwidget.cpp
    GUI/pythonmodule.cpp
    GUI/doublespinboxdelegate.cpp
    GUI/comboboxdelegate.cpp
    GUI/wellsproxymodel.cpp
    GUI/genpropwidget.cpp
    GUI/dateeditdelegate.cpp
    GUI/doublevalidatordelegate.cpp
    GUI/customoutputwindow.cpp
    GUI/initdatawidget.cpp
    GUI/tableview.cpp
    GUI/ttdelegate.cpp
    GUI/treeview.cpp
    GUI/viewsettingswidget.cpp
)

add_executable(vector
    ${CORE_CPP_SOURCES}
    ${GUI_CPP_SOURCES}
    main.cpp
    res.qrc
)

target_include_directories(vector PRIVATE
    ${Python3_INCLUDE_DIR}
    ${VTK_INCLUDE_DIRS}
    ${HDF5_INCLUDE_DIRS}
    ${Eigen3_INCLUDE_DIR}
    ${Eigen3_HDF5_INCLUDE_DIR}
)

target_link_libraries(vector PRIVATE
    Qt5::Widgets
    Qt5::OpenGL
    Qt5::Charts
    ${Python3_LIBRARY}
    ${HDF5_LIBRARIES}
    VTK::CommonColor
    VTK::CommonCore
    VTK::CommonDataModel
    VTK::FiltersGeometry
    VTK::FiltersSources
    VTK::InteractionStyle
    VTK::RenderingContextOpenGL2
    VTK::RenderingCore
    VTK::RenderingFreeType
    VTK::RenderingGL2PSOpenGL2
    VTK::RenderingOpenGL2
    VTK::GUISupportQt
    VTK::RenderingAnnotation
    VTK::RenderingVolume
    VTK::WrappingPythonCore
    VTK::PythonInterpreter
)

set_target_properties(vector PROPERTIES
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS vector
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

vtk_module_autoinit(
    TARGETS vector
    MODULES ${VTK_LIBRARIES}
)
