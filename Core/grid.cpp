#include "grid.h"

Grid::Grid() {}


int Grid::blockIndex(int i, int j, int k) const {
    return i + nx * (j + ny * k);
}


void Grid::setPressure(const Eigen::VectorXd& P) {
    this->P = P;
}


void Grid::setPoro(const Eigen::ArrayXd& poro) {
    this->poroinit = poro;
    this->poro = poro;
}


void Grid::setPermx(const Eigen::ArrayXd& permx) {
    this->permx = permx;
}


void Grid::setPermy(const Eigen::ArrayXd& permy) {
    this->permy = permy;
}


void Grid::setPermz(const Eigen::ArrayXd& permz) {
    this->permz = permz;
}


void Grid::setPVT(const PVT& pvt) {
    this->pvt = pvt;
}


void Grid::setRock(const Rock& rock) {
    this->rock = rock;
}


void Grid::addDt(double dt) {
    time+=dt;
}


int Grid::NX() {
    return nx;
}


int Grid::NY() {
    return ny;
}


int Grid::NZ() {
    return nz;
}

Eigen::SparseMatrix<double> Grid::getT() {
    return T;
}

Eigen::SparseMatrix<double> Grid::getD() {
    return D;
}

Eigen::VectorXd Grid::getP() {
    return P;
}

Eigen::VectorXd Grid::getQ() {
    return Q;
}

double Grid::currentTime() {
    return time;
}

double Grid::permxi(int i) {
    return permx[i];
}


double Grid::permyi(int i) {
    return permy[i];
}


double Grid::permzi(int i) {
    return permz[i];
}

double Grid::mu(int i) {
    return pvt.Mu(P)[i];
}


double Grid::xi(int i) {
    return x[i];
}


double Grid::yi(int i) {
    return y[i];
}


double Grid::zi(int i) {
    return z[i];
}


double Grid::dxi(int i) {
    return dx[i];
}


double Grid::dyi(int i) {
    return dy[i];
}


double Grid::dzi(int i) {
    return dz[i];
}


double &Grid::Pi(int i) {
    return P.coeffRef(i);
}


double &Grid::Qi(int i) {
    return Q.coeffRef(i);
}

double Grid::radius(int i) {
    return 0.28 * sqrt(sqrt(permxi(i) / permyi(i)) * pow(dxi(i), 2) + sqrt(permyi(i) / permxi(i)) * pow(dyi(i), 2)) / (pow(permyi(i) / permxi(i), 0.25) + pow(permxi(i) / permyi(i), 0.25));
}

double Grid::perm(int i) {
    return sqrt(pow(permyi(i),2) + pow(permyi(i),2) + pow(permzi(i),2));
}

Eigen::ArrayXd Grid::X() {
    return x;
}

Eigen::ArrayXd Grid::Y() {
    return y;
}

Eigen::ArrayXd Grid::Z() {
    return z;
}
