#include "pvtw.h"

// Вода.
PVTW::PVTW() {}


void PVTW::setData(const std::vector<double>& data) {
    pwref = data[0];
    bwref = data[1];
    cw = data[2];
    mwref = data[3];
    cvw = data[4];
    rhow = data[5];

    b.clear();
    m.clear();

    int n = p.size();

    b.resize(n);
    m.resize(n);

    for(int i = 0; i < n; ++i) {
        b[i] = B(p[i]);
        m[i] = Mu(p[i]);
    }
}


double PVTW::B(double p) {
    return bwref / (1 + cw * (p - pwref) + 0.5 * pow(cw * (p - pwref),2));
}


double PVTW::Mu(double p) {
    return mwref * exp(cvw * (p - pwref));
}


double PVTW::Rho(double p) {
    return rhow/B(p);
}


double PVTW::RhoSC() {
    return rhow;
}


double PVTW::C() {
    return cw;
}


Eigen::ArrayXd PVTW::B(const Eigen::ArrayXd& p) {
    return bwref / (1 + cw * (p - pwref) + 0.5 * (cw * (p - pwref)).pow(2));
}


Eigen::ArrayXd PVTW::Mu(const Eigen::ArrayXd& p) {
    return mwref * (cvw * (p - pwref)).exp();
}


Eigen::ArrayXd PVTW::Rho(const Eigen::ArrayXd &p) {
     return rhow/B(p);
}


std::vector<double> PVTW::values() {
    return {pwref,bwref,cw,mwref,cvw,rhow};
}
