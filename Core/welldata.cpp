#include "welldata.h"
#include <QDebug>
WellData::WellData() {

}

void WellData::update(double time) {
    for(auto& perf : perforations) {
        if(time >= perf.begin_time && time <= perf.ent_time) {
            perf.isStop = false;
        }
        else {
            perf.isStop = true;
        }
    }
}

void WellData::setTrajectory(const Eigen::Matrix<double, Eigen::Dynamic, 4> &welltrack) {
    this->welltrack = welltrack;
}

void WellData::setControls(const std::vector<Control>& controls) {
    this->controls = controls;
}

void WellData::sePerforation(const std::vector<Perforation>& perforations) {
    this->perforations = perforations;
}

Eigen::Matrix<double, Eigen::Dynamic, 4> WellData::getTrajectory() {
    return welltrack;
}

std::vector<Control> WellData::getControls() {
    return controls;
}

std::vector<Perforation>& WellData::getPerforations() {
    return perforations;
}
