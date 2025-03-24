#include "well.h"


Well::Well() {

}


void Well::setGrid(Grid* grid) {
    this->grid = grid;
}

void Well::setData(WellData* welldata) {
    this->welldata = welldata;
}


void Well::init() {
    Eigen::Matrix<double, Eigen::Dynamic, 4> welltrack = welldata->getTrajectory();
    for (int i = 0; i < welltrack.rows(); ++i) {
        int index = -1;
        ((grid->X() - welltrack(i,0)).pow(2) + (grid->Y() - welltrack(i,1)).pow(2) + (grid->Z() - welltrack(i,2)).pow(2)).sqrt().minCoeff(&index);

        for(auto& perf : welldata->getPerforations()) {
            if (grid->zi(index) >= perf.begin_tvd && grid->zi(index) <= perf.end_tvd) {
                perf.index = index;
            }
        }
    }
}


void Well::update() {
    double time = grid->currentTime();
    welldata->update(time);

    for(auto&  control : welldata->getControls()) {
        if(time < control.time) break;

        if(control.type == UnitType::PRESSURE) {
            for(auto& perf : welldata->getPerforations()) {
                if(perf.isStop) break;
                int i = perf.index;
                double rb = grid->radius(i);

                double wi = 2 * M_PI * perf.mult / ((log(rb/perf.radius) + perf.skin)*grid->mu(i));

                double perm = grid->perm(i);

                grid->Qi(i) = wi * perm * grid->dzi(i) * (grid->Pi(i) - control.value);
            }
        }
    }
}
