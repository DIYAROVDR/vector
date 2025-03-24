#include "rockdata.h"

RockData::RockData() {

}

RockData::RockData(double pref, double cpp) : pref(pref) , cpp(cpp) {

}

double RockData::PREF() {
    return pref;
}

double RockData::C() {
    return cpp;
}

std::vector<double> RockData::values() {
    return {pref, cpp, cr, cbc, fi0, nu0};
}

void RockData::setData(const std::vector<double> &data) {
    pref = data[0];
    cpp = data[1];
    cr = data[2];
    cbc = data[3];
    fi0 = data[4];
    nu0 = data[5];
}
