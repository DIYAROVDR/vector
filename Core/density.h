#ifndef DENSITY_H
#define DENSITY_H
#include <vector>

class Density {
public:
    Density();
    Density(double rhow, double rhoo,double rhog);
    void setData(double rhow, double rhoo,double rhog);
    double RHOW();
    double RHOO();
    double RHOG();
    std::vector<double> values();

private:
    double rhow = 1000.0; // Плостность воды
    double rhoo = 800.0;  // Плотность нефти
    double rhog = 1.0;    // Плотность газа
};

#endif // DENSITY_H
