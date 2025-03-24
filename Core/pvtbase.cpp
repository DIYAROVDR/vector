#include "pvtbase.h"


PVTBase::PVTBase() {
    p = {1E+5, 50E+5, 100E+5, 150E+5, 200E+5, 250E+5, 300E+5, 350E+5, 400E+5, 450E+5};
}

std::vector<double> PVTBase::ptab() {
    return p;
}

std::vector<double> PVTBase::btab() {
    return b;
}

std::vector<double> PVTBase::mutab() {
    return m;
}
