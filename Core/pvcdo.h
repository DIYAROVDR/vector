#ifndef PVCDO_H
#define PVCDO_H

#include "pvtbase.h"

// Дегазированная нефть с постоянной сжимаемостью.
class PVCDO : public PVTBase {
public:
    PVCDO();

    void setData(const std::vector<double>& data) override;

    double B(double p) override;
    double Mu(double p) override;
    double Rho(double p) override;
    double RhoSC() override;
    double C() override;

    Eigen::ArrayXd B(const Eigen::ArrayXd& p) override;
    Eigen::ArrayXd Mu(const Eigen::ArrayXd& p) override;
    Eigen::ArrayXd Rho(const Eigen::ArrayXd& p) override;

    std::vector<double> values() override;

private:
    double poref = 200E+5;
    double boref = 1.0;
    double co = 1.0E-9;
    double moref = 5E-3;
    double cvo = 1.0E-9;
    double rs = 0.0;
    double pbub = 1.0E+5;
    double rhoo = 800.0;
    double rhog = 1.0;
};
#endif // PVCDO_H
