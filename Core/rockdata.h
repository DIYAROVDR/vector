#ifndef ROCKDATA_H
#define ROCKDATA_H

#include <vector>

class RockData {
    double pref = 200E+5;   // опорное давление
    double cpp = 1.0E-9;    // сжимаемость породы
    double cr = 0.0;        // сжимаемость скелета породы
    double cbc = 0.0;       // сжимаемость блока (блока, содержащего смесь)
    double fi0 = 0.33;      // значение пористости при опорном давлении
    double nu0 = 0.0;       // значение коэфициента Пуассона

public:
    RockData();
    RockData(double pref, double cpp);
    double PREF();
    double C();

    std::vector<double> values();
    void setData(const std::vector<double>& data);
};

#endif // ROCKDATA_H
