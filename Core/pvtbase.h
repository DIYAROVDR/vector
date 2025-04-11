#ifndef PVTBASE_H
#define PVTBASE_H

#include "Eigen/Dense"

enum class FluidType {
    WATER,
    OIL,
    GAS
};

class PVTBase {
public:
    PVTBase();
    virtual ~PVTBase() = default;

    virtual double B(double p) = 0;
    virtual double Mu(double p) = 0;
    virtual double Rho(double p) = 0;
    virtual double C() = 0;
    virtual double RhoSC() = 0;

    virtual Eigen::ArrayXd B(const Eigen::ArrayXd& p) = 0;
    virtual Eigen::ArrayXd Mu(const Eigen::ArrayXd& p) = 0;
    virtual Eigen::ArrayXd Rho(const Eigen::ArrayXd& p) = 0;

    virtual void setData(const std::vector<double>& data) = 0;

    Eigen::ArrayXd ptab();
    Eigen::ArrayXd btab();
    Eigen::ArrayXd mutab();

    virtual std::vector<double> values() = 0;
protected:
    Eigen::ArrayXd p;
    Eigen::ArrayXd b;
    Eigen::ArrayXd m;
};

#endif // PVTBASE_H
