#ifndef WELL_H
#define WELL_H

#include <math.h>
#include <cstring>
#include <algorithm>
#include <vector>

#include "../GUI/units.h"
#include "blockcenteredgrid.h"
#include "welldata.h"

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
