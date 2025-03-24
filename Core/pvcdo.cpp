#include "pvcdo.h"

// Дегазированная нефть с постоянной сжимаемостью.
PVCDO::PVCDO() {}


void PVCDO::setData(const std::vector<double>& data) {
    if (data.size() != 9) {
        // return;
    }

    poref = data[0];    // Опорное давление
    boref = data[1];    // Объемный коэффициент при давлении poRef
    co = data[2];       // Сжимаемость нефти
    moref = data[3];    // Вязкость нефти при давлении poRef
    cvo = data[4];      // Градиент вязкости нефти
    rs = data[5];       // Газосодержание
    pbub = data[6];     // Давление насыщения
    rhoo = data[7];
    rhog = data[8];


    b.clear();
    m.clear();

    int n = p.size();

    b.resize(n);
    m.resize(n);

    for (int i = 0; i < n; ++i) {
        b[i] = B(p[i]);
        m[i] = Mu(p[i]);
    }
}

double PVCDO::B(double p) {
    return boref * exp(-co * (p - poref));
}

double PVCDO::Mu(double p) {
    return moref * exp(cvo * (p - poref));
}

double PVCDO::Rho(double p) {
    return (rs*rhog + rhoo)/B(p);
}

double PVCDO::RhoSC() {
    return rhoo;
}

double PVCDO::C() {
    return co;
}

Eigen::ArrayXd PVCDO::B(const Eigen::ArrayXd& p) {
    return moref * (-co * (p - poref)).exp();
}

Eigen::ArrayXd PVCDO::Mu(const Eigen::ArrayXd& p) {
    return moref * (cvo * (p - poref)).exp();
}

Eigen::ArrayXd PVCDO::Rho(const Eigen::ArrayXd& p) {
    return  (rs*rhog + rhoo)/B(p);
}

std::vector<double> PVCDO::values() {
    return {poref,boref,co,moref,cvo,rs,pbub,rhoo,rhog};
}

