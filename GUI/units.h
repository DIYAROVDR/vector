#ifndef UNITS_H
#define UNITS_H

#include <QString>
#include <QMap>
#include <vector>
#include <stdexcept>

// Определение enum class для типов единиц измерения
enum class UnitType {
    PRESSURE,                // Давление
    PERMEABILITY,            // Проницаемость
    VISCOSITY,               // Вязкость
    COMPRESSIBILITY,         // Сжимаемость
    RATE,                    // Дебит
    LENGTH,                  // Длина
    POROSITY,                // Пористость
    VOLUMETRIC_EXPANSION,    // Коэффициент объемного расширения
    GAS_CONTENT,             // Газосодержание
    VISCOSITY_GRADIENT,      // Градиент вязкости
    DENSITY,                 // Плотность
    POISSON_RATIO,           // Коэффициент Пуассона
    TVD,                     // Абсолютная глубина
    MD,                      // Измеренная глубина
    LIQUI_RATE,              // Дебит жидкости
    WATER_RATE,              // Дебит воды
    OIL_RATE,                // Дебит нефти
    GAS_RATE,                // Дебит газа
};


class Units {
private:
    static const QMap<QString, UnitType> unitnames;
public:
    Units();

    // Преобразование из TS в SI
    static double SI(double value, UnitType unit);

    // Преобразование из SI в TS
    static double TS(double value, UnitType unit);

    // Преобразование вектора из TS в SI
    static std::vector<double> SI(const std::vector<double>& values, UnitType unit);

    // Преобразование вектора из SI в TS
    static std::vector<double> TS(const std::vector<double>& values, UnitType unit);

    // Возвращает название единицы измерения для TS
    static QString unitNameTS(UnitType unit);

    // Возвращает название единицы измерения для SI
    static QString unitNameSI(UnitType unit);

    static QString valueName(UnitType unit);

    static UnitType valueType(const QString& name);
};




#endif // UNITS_H
