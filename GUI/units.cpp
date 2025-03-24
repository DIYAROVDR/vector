#include "units.h"

Units::Units() {}

const QMap<QString, UnitType> Units::unitnames = {
    {"Давление", UnitType::PRESSURE},
    {"Проницаемость", UnitType::PERMEABILITY},
    {"Вязкость", UnitType::VISCOSITY},
    {"Сжимаемость", UnitType::COMPRESSIBILITY},
    {"Дебит", UnitType::RATE},
    {"Длина", UnitType::LENGTH},
    {"Пористость", UnitType::POROSITY},
    {"Коэффициент объемного расширения", UnitType::VOLUMETRIC_EXPANSION},
    {"Газосодержание", UnitType::GAS_CONTENT},
    {"Градиент вязкости", UnitType::VISCOSITY_GRADIENT},
    {"Плотность", UnitType::DENSITY},
    {"Коэффициент Пуассона", UnitType::POISSON_RATIO},
    {"Абсолютная глубина", UnitType::TVD},
    {"Измеренная глубина", UnitType::MD},
    {"Дебит жидкости", UnitType::LIQUI_RATE},
    {"Дебит воды", UnitType::WATER_RATE},
    {"Дебит нефти", UnitType::OIL_RATE},
    {"Дебит газа", UnitType::GAS_RATE}
};

// Преобразование из TS в SI
double Units::SI(double value, UnitType unit) {
    switch (unit) {
        case UnitType::PRESSURE:             // Давление
            return value * 1e5;              // Бар -> Па
        case UnitType::PERMEABILITY:         // Проницаемость
            return value * 9.869233e-16;     // мДарси -> м^2
        case UnitType::VISCOSITY:            // Вязкость
            return value * 1e-3;             // сПз -> Па*с
        case UnitType::COMPRESSIBILITY:      // Сжимаемость
            return value * 1e-5;             // 1/Бар -> 1/Па
        case UnitType::RATE:                 // Дебит (расход)
            return value / 86400;            // м3/сут -> м3/с
        case UnitType::LENGTH:               // Длина
            return value;                    // метры -> метры (без изменений)
        case UnitType::POROSITY:             // Пористость
            return value;                    // Доли единиц -> Доли единиц (без изменений)
        case UnitType::VOLUMETRIC_EXPANSION: // Коэффициент объемного расширения
            return value;                    // Доли единиц -> Доли единиц (без изменений)
        case UnitType::GAS_CONTENT:          // Газосодержание
            return value;                    // Доли единиц -> Доли единиц (без изменений)
        case UnitType::VISCOSITY_GRADIENT:   // Градиент вязкости
            return value * 1e-5;             // 1/Бар -> 1/Па
        case UnitType::DENSITY:              // Плотность
            return value;                    // кг/м³ -> кг/м³ (без изменений)
        case UnitType::POISSON_RATIO:        // Коэффициент Пуассона
            return value;                    // Доли единиц -> Доли единиц (без изменений)
        default:
            throw std::invalid_argument("Unknown unit type");
    }
}

// Преобразование из SI в TS
double Units::TS(double value, UnitType unit) {
    switch (unit) {
        case UnitType::PRESSURE:             // Давление
            return value / 1e5;              // Па -> Бар
        case UnitType::PERMEABILITY:         // Проницаемость
            return value / 9.869233e-16;     // м^2 -> мДарси
        case UnitType::VISCOSITY:            // Вязкость
            return value / 1e-3;             // Па*с -> сПз
        case UnitType::COMPRESSIBILITY:      // Сжимаемость
            return value / 1e-5;             // 1/Па -> 1/Бар
        case UnitType::RATE:                 // Дебит (расход)
            return value * 86400;            // м3/с -> м3/сут
        case UnitType::LENGTH:               // Длина
            return value;                    // метры -> метры (без изменений)
        case UnitType::POROSITY:             // Пористость
            return value;                    // Доли единиц -> Доли единиц (без изменений)
        case UnitType::VOLUMETRIC_EXPANSION: // Коэффициент объемного расширения
            return value;                    // Доли единиц -> Доли единиц (без изменений)
        case UnitType::GAS_CONTENT:          // Газосодержание
            return value;                    // Доли единиц -> Доли единиц (без изменений)
        case UnitType::VISCOSITY_GRADIENT:   // Градиент вязкости
            return value * 1e5;              // 1/Па -> 1/Бар
        case UnitType::DENSITY:              // Плотность
            return value;                    // кг/м³ -> кг/м³ (без изменений)
        case UnitType::POISSON_RATIO:        // Коэффициент Пуассона
            return value;                    // Доли единиц -> Доли единиц (без изменений)
        default:
            throw std::invalid_argument("Unknown unit type");
    }
}

// Преобразование вектора из TS в SI
std::vector<double> Units::SI(const std::vector<double>& values, UnitType unit) {
    std::vector<double> result;
    result.reserve(values.size());
    for (const auto& value : values) {
        result.push_back(SI(value, unit));
    }
    return result;
}

// Преобразование вектора из SI в TS
std::vector<double> Units::TS(const std::vector<double>& values, UnitType unit) {
    std::vector<double> result;
    result.reserve(values.size());
    for (const auto& value : values) {
        result.push_back(TS(value, unit));
    }
    return result;
}

// Возвращает название единицы измерения для TS
QString Units::unitNameTS(UnitType unit) {
    switch (unit) {
        case UnitType::PRESSURE:
            return "Бар";
        case UnitType::PERMEABILITY:
            return "мДарси";
        case UnitType::VISCOSITY:
            return "сПз";
        case UnitType::COMPRESSIBILITY:
            return "1/Бар";
        case UnitType::RATE:
            return "м³/сут";
        case UnitType::LENGTH:
            return "м";
        case UnitType::POROSITY:
            return "д. ед";
        case UnitType::VOLUMETRIC_EXPANSION:
            return "м³/ст.м³";
        case UnitType::GAS_CONTENT:
            return "ст.м³/ст.м³";
        case UnitType::VISCOSITY_GRADIENT:
            return "1/Бар";
        case UnitType::DENSITY:
            return "кг/м³";
        case UnitType::POISSON_RATIO:
            return "д. ед";
        default:
            throw std::invalid_argument("Unknown unit type");
    }
}

// Возвращает название единицы измерения для SI
QString Units::unitNameSI(UnitType unit) {
    switch (unit) {
        case UnitType::PRESSURE:
            return "Па";
        case UnitType::PERMEABILITY:
            return "м2";
        case UnitType::VISCOSITY:
            return "Па*с";
        case UnitType::COMPRESSIBILITY:
            return "1/Па";
        case UnitType::RATE:
            return "м³/с";
        case UnitType::LENGTH:
            return "м";
        case UnitType::POROSITY:
            return "д. ед";
        case UnitType::VOLUMETRIC_EXPANSION:
            return "м³/ст.м³";
        case UnitType::GAS_CONTENT:
            return "ст.м³/ст.м³";
        case UnitType::VISCOSITY_GRADIENT:
            return "1/Па";
        case UnitType::DENSITY:
            return "кг/м³";
        case UnitType::POISSON_RATIO:
            return "д. ед";
        default:
            throw std::invalid_argument("Unknown unit type");
    }
}

QString Units::valueName(UnitType unit) {
    return unitnames.key(unit);
}

UnitType Units::valueType(const QString& name) {
    return unitnames.value(name);
}

