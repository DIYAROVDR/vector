#include "density.h"

Density::Density() {

}

Density::Density(double rhow, double rhoo, double rhog):rhow(rhow), rhoo(rhoo), rhog(rhog) {

}

void Density::setData(double rhow, double rhoo, double rhog) {
    this->rhow = rhow;
    this->rhoo = rhoo;
    this->rhog = rhog;
}

double Density::RHOW() {
    return rhow;
}

double Density::RHOO() {
    return rhoo;
}

double Density::RHOG() {
    return rhog;
}

std::vector<double> Density::values() {
    return {rhow, rhoo, rhog};
}
