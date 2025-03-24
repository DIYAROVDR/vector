#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <map>

#include <Eigen/Dense>

class Unit {
public:
    enum class Types {
        AREA,                  // Площадь
        COMPRESSIBILITY,       // Сжимаемость
        DENSITY,               // Плотность
        GAS_CONTENT,           // Содержание газа
        OIL_CONTENT,           // Содержание нефти
        LENGTH,                // Длина
        PERMEABILITY,          // Проницаемость
        POISSON_RATIO,         // Коэффициент Пуассона
        POROSITY,              // Пористость
        PRESSURE,              // Давление
        RATE,                  // Дебит
        SATURATION,            // Насыщенность
        TEMPERATURE,           // Температура
        VISCOSITY,             // Вязкость
        VISCOSITY_GRADIENT,    // Градиент вязкости
        VOLUMETRIC_EXPANSION,  // Объемное расширение
        VOLUME,                 // Объем
        DIMENSIONLESS,
        UNDEFINED
    };
    enum class System {
        SI,
        TS
    };
protected:
    System outsys = System::TS;
    System insys = System::SI;
    std::map<Types, std::string> unitname;
    std::map<System,std::string> unitsystemname;
    std::map<System, std::map<Types, std::string>> unitvalue;
    std::map<System, std::map<System, std::map<Types, double>>> convcoeff;
public:
    Unit();
    void setUnitSystem(System s);
    System currentUnitSystem();

    std::string systemName(System system);
    System systemValue(const std::string& name);

    Eigen::ArrayXd convert(Eigen::ArrayXd& array, Types type);
    Eigen::ArrayXd diconvert(Eigen::ArrayXd& array, Types type);
};

#endif // UNIT_H
