#ifndef ROCK_H
#define ROCK_H

#include "../Core/rockdata.h"
#include "Eigen/Dense"

class Rock {
public:
    Rock();
    void setRegionCount(int count);
    void setRegions(const Eigen::ArrayXi& cube);
    void setRock(size_t index, const RockData& rockdata);

    Eigen::ArrayXd C();

    // RockData& operator[](size_t index) const;
private:
    int regcount = 1;
    Eigen::ArrayXi regions;
    std::vector<RockData> rocktabs;
};

#endif // ROCK_H
