cmake_minimum_required(VERSION 3.16)
project(vector VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(QT_VERSION_MAJOR 5)

if(UNIX)
    set(Eigen3_INCLUDE_DIR "/home/diyarovdr/lib/eigen-3.4.0")
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


# Ищем только Qt 5
find_package(Qt5 REQUIRED COMPONENTS
    Widgets
    PrintSupport
    OpenGL
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
  IOHDF
)

file(GLOB MODEL_SOURCES ${CMAKE_SOURCE_DIR}/Core/*.cpp ${CMAKE_SOURCE_DIR}/Core/*.hpp ${CMAKE_SOURCE_DIR}/Core/*.h)
file(GLOB DATA_SOURCES ${CMAKE_SOURCE_DIR}/GUI/*.cpp ${CMAKE_SOURCE_DIR}/GUI/*.h ${CMAKE_SOURCE_DIR}/GUI/*.ui)

set(PROJECT_SOURCES
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${MODEL_SOURCES}
    ${DATA_SOURCES}
    ${HELPERS_SOURCES}
    ${SOLVER_SOURCES}
    ${VIEW_SOURCES}
)

# Создаем исполняемый файл
add_executable(vector
    ${PROJECT_SOURCES}
    res.qrc
)

target_include_directories(vector PRIVATE
    ${Python3_INCLUDE_DIR}
    ${VTK_INCLUDE_DIRS}
    ${HDF5_INCLUDE_DIRS}
    ${Eigen3_INCLUDE_DIR}
)

target_link_libraries(vector PRIVATE
  Qt5::Widgets
  Qt5::OpenGL
  Qt5::PrintSupport
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
  VTK::IOHDF
)

if(WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(ENV{PATH} "$ENV{PATH};c:/Program Files (x86)/Intel/oneAPI/tbb/2022.0/bin/vc_mt")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(ENV{PATH} "$ENV{PATH};c:/Program Files (x86)/Intel/oneAPI/tbb/2022.0/lib/vc_mt")
    endif()
endif()

target_link_directories(vector PRIVATE "c:/Program Files (x86)/Intel/oneAPI/tbb/2022.0/bin/vc_mt")
target_link_libraries(vector PRIVATE tbb12.lib tbb12.dll)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Release configuration
    target_include_directories(vector PRIVATE "c:/Program Files (x86)/Intel/oneAPI/tbb/2022.0/include")
    target_link_directories(vector PRIVATE "c:/Program Files (x86)/Intel/oneAPI/tbb/2022.0/bin/vc_mt")
    target_link_directories(vector PRIVATE "c:/Program Files (x86)/Intel/oneAPI/tbb/2022.0/lib/vc_mt")
    target_link_libraries(vector PRIVATE tbb12.lib tbb12.dll)
    target_include_directories(vector PRIVATE "c:/Program Files (x86)/Intel/oneAPI/mkl/2025.0/include")
    target_link_directories(vector PRIVATE "c:/Program Files (x86)/Intel/oneAPI/mkl/2025.0/lib")
    target_link_libraries(vector PRIVATE mkl_intel_lp64 mkl_tbb_thread mkl_core mkl_rt)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Debug configuration
    target_include_directories(vector PRIVATE "c:/Program Files (x86)/Intel/oneAPI/tbb/2022.0/include")
    target_link_directories(vector PRIVATE "c:/Program Files (x86)/Intel/oneAPI/tbb/2022.0/lib/vc_mt")
    target_link_libraries(vector PRIVATE tbb)
    target_include_directories(vector PRIVATE "c:/Program Files (x86)/Intel/oneAPI/mkl/2025.0/include")
    target_link_directories(vector PRIVATE "c:/Program Files (x86)/Intel/oneAPI/mkl/2025.0/lib")
    target_link_libraries(vector PRIVATE mkl_intel_lp64_dll mkl_tbb_thread_dll mkl_core_dll mkl_rt)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D EIGEN_USE_MKL_ALL /D EIGEN_USE_PARDISO")




set_target_properties(vector PROPERTIES
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS vector
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Инициализация VTK
vtk_module_autoinit(
  TARGETS vector
  MODULES ${VTK_LIBRARIES}
)
