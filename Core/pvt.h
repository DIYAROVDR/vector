#ifndef PVT_H
#define PVT_H

#include "pvtbase.h"

class PVT {
public:
    enum class Fluid {
        WATER,
        OIL,
        GAS,
        DISGAS,
        VAPOIL
    };
public:
    PVT();
    void setRegionCount(int count);
    void setRegions(const Eigen::ArrayXi& cube);
    void setPVT(size_t index, PVTBase* pvt);

    Eigen::ArrayXd B(const Eigen::ArrayXd& p);
    Eigen::ArrayXd Mu(const Eigen::ArrayXd& p);
    Eigen::ArrayXd C();

    PVTBase* operator[](size_t index) const;
private:
    int regcount = 1;
    Eigen::ArrayXi regions;
    std::vector<PVTBase*> pvttabs;
};

#endif // PVT_H
