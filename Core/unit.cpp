#include "unit.h"

Unit::Unit() {
    /**
     * Инициализация словаря соответствий:
     * ключ - тип единицы измерения (Unit::Types)
     * значение - название единицы измерения (std::string)
     */
    unitname = {
        {Types::AREA, "Площадь"},
        {Types::COMPRESSIBILITY, "Сжимаемость"},
        {Types::DENSITY, "Плотность"},
        {Types::GAS_CONTENT, "Содержание газа"},
        {Types::OIL_CONTENT, "Содержание нефти"},
        {Types::LENGTH, "Длина"},
        {Types::PERMEABILITY, "Проницаемость"},
        {Types::POISSON_RATIO, "Коэффициент Пуассона"},
        {Types::POROSITY, "Пористость"},
        {Types::PRESSURE, "Давление"},
        {Types::RATE, "Дебит"},
        {Types::SATURATION, "Насыщенность"},
        {Types::TEMPERATURE, "Температура"},
        {Types::VISCOSITY, "Вязкость"},
        {Types::VISCOSITY_GRADIENT, "Градиент вязкости"},
        {Types::VOLUMETRIC_EXPANSION, "Объемное расширение"},
        {Types::VOLUME, "Объем"},
        {Types::DIMENSIONLESS, "Безразмерная величина"},
        {Types::UNDEFINED, "Не определена"}
    };

    unitsystemname = {
        {System::TS, "Промысловая"},
        {System::SI, "СИ"}
    };

    // Инициализация карты единиц измерения для системы SI
    unitvalue[System::SI] = {
        {Types::AREA, "м²"},
        {Types::COMPRESSIBILITY, "1/Па"},
        {Types::DENSITY, "кг/м³"},
        {Types::GAS_CONTENT, "доли ед."},
        {Types::OIL_CONTENT, "доли ед."},
        {Types::LENGTH, "м"},
        {Types::PERMEABILITY, "м²"},
        {Types::POISSON_RATIO, "доли ед."},
        {Types::POROSITY, "доли ед."},
        {Types::PRESSURE, "Па"},
        {Types::RATE, "м³/с"},
        {Types::SATURATION, "доли ед."},
        {Types::TEMPERATURE, "К"},
        {Types::VISCOSITY, "Па·с"},
        {Types::VISCOSITY_GRADIENT, "1/Па"},
        {Types::VOLUMETRIC_EXPANSION, "доли ед."},
        {Types::VOLUME, "м³"},
        {Types::DIMENSIONLESS, ""},
        {Types::UNDEFINED, ""}
    };

    // Инициализация карты единиц измерения для системы TS
    unitvalue[System::TS] = {
        {Types::AREA, "м²"},
        {Types::COMPRESSIBILITY, "1/атм"},
        {Types::DENSITY, "кг/м³"},
        {Types::GAS_CONTENT, "доли ед."},
        {Types::OIL_CONTENT, "доли ед."},
        {Types::LENGTH, "м"},
        {Types::PERMEABILITY, "мДарси"},
        {Types::POISSON_RATIO, "доли ед."},
        {Types::POROSITY, "доли ед."},
        {Types::PRESSURE, "атм"},
        {Types::RATE, "м³/сут"},
        {Types::SATURATION, "доли ед."},
        {Types::TEMPERATURE, "°C"},
        {Types::VISCOSITY, "сП"},
        {Types::VISCOSITY_GRADIENT, "1/атм"},
        {Types::VOLUMETRIC_EXPANSION, "доли ед."},
        {Types::VOLUME, "м³"},
        {Types::DIMENSIONLESS, ""},
        {Types::UNDEFINED, ""}
    };

    // Коэффициенты перевода из SI в TS
    convcoeff[System::SI][System::TS] = {
        {Types::AREA, 1.0},
        {Types::COMPRESSIBILITY,  101325.0}, // 1/Па -> 1/атм
        {Types::DENSITY, 1.0},
        {Types::GAS_CONTENT, 1.0},
        {Types::OIL_CONTENT, 1.0},
        {Types::LENGTH, 1.0},
        {Types::PERMEABILITY, 1.01325e15}, // м² -> мДарси
        {Types::POISSON_RATIO, 1.0},
        {Types::POROSITY, 1.0},
        {Types::PRESSURE, 1.0 / 101325.0}, // Па -> атм
        {Types::RATE, 86400.0}, // м³/с -> м³/сут
        {Types::SATURATION, 1.0},
        {Types::TEMPERATURE, -273.15}, // К -> °C
        {Types::VISCOSITY, 1000.0}, // Па·с -> сП
        {Types::VISCOSITY_GRADIENT,  101325.0}, // 1/Па -> 1/атм
        {Types::VOLUMETRIC_EXPANSION, 1.0},
        {Types::VOLUME, 1.0},
        {Types::DIMENSIONLESS, 1.0},
        {Types::UNDEFINED, 1.0}
    };

    // Коэффициенты перевода из TS в SI
    convcoeff[System::TS][System::SI] = {
        {Types::AREA, 1.0},
        {Types::COMPRESSIBILITY, 1/101325.0}, // 1/атм -> 1/Па
        {Types::DENSITY, 1.0},
        {Types::GAS_CONTENT, 1.0},
        {Types::OIL_CONTENT, 1.0},
        {Types::LENGTH, 1.0},
        {Types::PERMEABILITY, 9.869233e-16}, // мДарси -> м²
        {Types::POISSON_RATIO, 1.0},
        {Types::POROSITY, 1.0},
        {Types::PRESSURE, 101325.0}, // атм -> Па
        {Types::RATE, 1.0 / 86400.0}, // м³/сут -> м³/с
        {Types::SATURATION, 1.0},
        {Types::TEMPERATURE, 273.15}, // °C -> K
        {Types::VISCOSITY, 0.001}, // сП -> Па·с
        {Types::VISCOSITY_GRADIENT, 1/101325.0}, // 1/атм -> 1/Па
        {Types::VOLUMETRIC_EXPANSION, 1.0},
        {Types::VOLUME, 1.0},
        {Types::DIMENSIONLESS, 1.0},
        {Types::UNDEFINED, 1.0}
    };
}


void Unit::setUnitSystem(System s) {
    outsys = s;
    if (outsys == System::SI){
        insys = System::TS;
    }
    else {
        insys = System::SI;
    }
}


std::string Unit::systemName(System system) {
    return unitsystemname[system];
}


Unit::System Unit::systemValue(const std::string& name) {
    for (const auto& pair : unitsystemname) {
        if (pair.second == name) {
            return pair.first;
        }
    }
    return System::TS;
}


Unit::System Unit::currentUnitSystem() {
    return outsys;
}


Eigen::ArrayXd Unit::convert(Eigen::ArrayXd& array, Types type) {
    return array*convcoeff[outsys][insys][type];
}


Eigen::ArrayXd Unit::diconvert(Eigen::ArrayXd& array, Types type) {
    return array*convcoeff[insys][outsys][type];
}


double Unit::diconvert(double value, Unit::Types type) {
    return value*convcoeff[insys][outsys][type]; // из промысловой системы в СИ
}


double Unit::convert(double value, Unit::Types type) {
    return value*convcoeff[outsys][insys][type];
}
