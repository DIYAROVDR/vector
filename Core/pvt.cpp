#include "pvt.h"


PVT::PVT() : regions(Eigen::VectorXi::Zero(1)) {
    pvttabs.resize(regcount, nullptr);
}


void PVT::setRegionCount(int count) {
    regcount = count;
    pvttabs.resize(count);
    pvttabs.resize(count, nullptr);
}


void PVT::setRegions(const Eigen::ArrayXi& cube) {
    regions = cube;
}



void PVT::setPVT(size_t index, PVTBase* pvt) {
    if (index >= regcount) {
        throw std::out_of_range("Индекс вышел за пределы количества регионов");
    }

    delete pvttabs[index];

    pvttabs[index] = pvt;
}


Eigen::ArrayXd PVT::B(const Eigen::ArrayXd& p) {
    Eigen::ArrayXd result(p.size());

    for (int i = 0; i < regcount; ++i) {
        Eigen::Array<bool, Eigen::Dynamic, 1> valid = (regions == i + 1);

        Eigen::ArrayXd regionalResult = (*pvttabs[i]).B(p);

        result = valid.select(regionalResult, result);
    }

    return result;
}

Eigen::ArrayXd PVT::Mu(const Eigen::ArrayXd& p) {
    Eigen::ArrayXd result(p.size());

    for (int i = 0; i < regcount; ++i) {
        Eigen::Array<bool, Eigen::Dynamic, 1> valid = (regions == i + 1);

        Eigen::ArrayXd regionalResult = (*pvttabs[i]).Mu(p);

        result = valid.select(regionalResult, result);
    }

    return result;
}

Eigen::ArrayXd PVT::C() {
    Eigen::ArrayXd result(regions.size());

    for (int i = 0; i < regcount; ++i) {
        Eigen::Array<bool, Eigen::Dynamic, 1> valid = (regions == i + 1);

        Eigen::ArrayXd regionalResult(regions.size());

        regionalResult.setConstant((*pvttabs[i]).C());

        result = valid.select(regionalResult, result);
    }

    return result;
}


PVTBase* PVT::operator[](size_t index) const {
    if (index >= pvttabs.size()) {
        throw std::out_of_range("Index out of bounds in PVT::operator[]");
    }
    return pvttabs[index];
}
