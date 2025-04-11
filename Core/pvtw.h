#ifndef PVTW_H
#define PVTW_H

#include "pvtbase.h"

class PVTW : public PVTBase {
public:
    PVTW();

    void setData(const std::vector<double>& data) override;

    double B(double p) override;
    double Mu(double p) override;
    double Rho(double p) override;
    double RhoSC() override;
    double C() override;

    Eigen::ArrayXd B(const Eigen::ArrayXd& p) override;
    Eigen::ArrayXd Mu(const Eigen::ArrayXd& p) override;
    Eigen::ArrayXd Rho(const Eigen::ArrayXd& p) override;

    void setPwRef(double value);
    void setBwRef(double value);
    void setCw(double value);
    void setMwRef(double value);
    void setCvw(double value);
    void setRhoW(double value);

    double getPwRef();
    double getBwRef();
    double getCw();
    double getMwRef();
    double getCvw();
    double getRhoW();

    void update();


    std::vector<double> values() override;

private:
    double pwref = 200E+5;
    double bwref = 1.0;
    double cw = 1.0E-9;
    double mwref = 1.0E-3;
    double cvw = 1.0E-9;
    double rhow = 1000.0;
};
#endif // PVTW_H
