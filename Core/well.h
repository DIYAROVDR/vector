#ifndef WELL_H
#define WELL_H

#include "../Core/blockcenteredgrid.h"
#include "../Core/welldata.h"

#define M_PI 3.14159265358979323846

class Well {
private:
    Grid* grid = nullptr;
    WellData* welldata = nullptr;
public:
    Well();
    void setGrid(Grid* grid);
    void setData(WellData* welldata);
    void init();
    void update();
};

#endif // WELL_H
