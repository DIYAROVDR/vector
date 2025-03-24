#include "rock.h"


Rock::Rock() : regions(Eigen::VectorXi::Zero(1)) {
    rocktabs.resize(regcount);
}

void Rock::setRegionCount(int count) {
    regcount = count;
    rocktabs.resize(count);
}


void Rock::setRegions(const Eigen::ArrayXi& cube) {
    regions = cube;
}


void Rock::setRock(size_t index, const RockData& rockdata) {
    if (index >= regcount) {
        throw std::out_of_range("Индекс вышел за пределы количества регионов");
    }

    rocktabs[index] = rockdata;
}


Eigen::ArrayXd Rock::C() {
    Eigen::ArrayXd result(regions.size());

    for (int i = 0; i < regcount; ++i) {
        Eigen::Array<bool, Eigen::Dynamic, 1> valid = (regions == i + 1);

        Eigen::ArrayXd regionalResult(regions.size());

        regionalResult.setConstant((rocktabs[i]).C());

        result = valid.select(regionalResult, result);
    }

    return result;
}
