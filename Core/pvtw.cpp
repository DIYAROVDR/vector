#include "pvtw.h"

// Вода.
PVTW::PVTW() {
    update();
}


void PVTW::setData(const std::vector<double>& data) {
    pwref = data[0];
    bwref = data[1];
    cw = data[2];
    mwref = data[3];
    cvw = data[4];
    rhow = data[5];

    b.resize(0);
    m.resize(0);

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

void PVTW::setPwRef(double value) {
    pwref = value;
}

void PVTW::setBwRef(double value) {
    bwref = value;
}

void PVTW::setCw(double value) {
    cw = value;
}

void PVTW::setMwRef(double value) {
    mwref = value;
}

void PVTW::setCvw(double value) {
    cvw = value;
}

void PVTW::setRhoW(double value) {
    rhow = value;
}
double PVTW::getPwRef() {
    return pwref;
}

double PVTW::getBwRef() {
    return bwref;
}

double PVTW::getCw() {
    return cw;
}

double PVTW::getMwRef() {
    return mwref;
}

double PVTW::getCvw() {
    return cvw;
}

double PVTW::getRhoW() {
    return rhow;
}

void PVTW::update() {
    int n = p.size();

    b.resize(n);
    m.resize(n);

    for(int i = 0; i < n; ++i) {
        b[i] = B(p[i]);
        m[i] = Mu(p[i]);
    }
}


std::vector<double> PVTW::values() {
    return {pwref,bwref,cw,mwref,cvw,rhow};
}
