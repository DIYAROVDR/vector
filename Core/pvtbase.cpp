#include "pvtbase.h"

PVTBase::PVTBase() {
    p = Eigen::ArrayXd::LinSpaced(10, 1E+5, 450E+5);
}

Eigen::ArrayXd PVTBase::ptab() {
    return p;
}

Eigen::ArrayXd PVTBase::btab() {
    return b;
}

Eigen::ArrayXd PVTBase::mutab() {
    return m;
}
