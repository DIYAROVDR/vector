#ifndef PVDO_H
#define PVDO_H

#include "pvtbase.h"

/*
class PVDO : public PVTBase {
public:
    PVDO();

    void setData(const std::vector<double>& data) override;

    double B(double p) override;
    double Mu(double p) override;
    double Rho(double p) override;
    double C() override;
    double RhoSC() override;

    Eigen::VectorXd B(const Eigen::VectorXd& p) override;
    Eigen::VectorXd Mu(const Eigen::VectorXd& p) override;
    Eigen::VectorXd Rho(const Eigen::VectorXd& p) override;

    std::vector<double> values() override;

private:
    double rhoo = 800.0;
};
*/
#endif // PVDO_H
