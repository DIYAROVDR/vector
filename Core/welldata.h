#ifndef WELLDATA_H
#define WELLDATA_H

#include "Eigen/Dense"
#include "../GUI/units.h"

struct Control {
    UnitType type = UnitType::PRESSURE;
    double time = 0.0;
    double value = 0.0;
};


struct Perforation {
    bool isStop = false;
    double begin_time = 0.0;
    double ent_time = 0.0;
    double begin_tvd = 0.0;
    double end_tvd = 0.0;
    double radius = 0.1;
    double skin = 0.0;
    double mult = 1.0;
    int index = 0;
};


class WellData {
public:
    WellData();
    void update(double time);
    void setTrajectory(const Eigen::Matrix<double, Eigen::Dynamic, 4>& welltrack);
    void setControls(const std::vector<Control>& controls);
    void sePerforation(const std::vector<Perforation>& perforations);

    Eigen::Matrix<double, Eigen::Dynamic, 4> getTrajectory();
    std::vector<Control> getControls();
    std::vector<Perforation>& getPerforations();
private:
    Eigen::Matrix<double, Eigen::Dynamic, 4> welltrack;
    std::vector<Control> controls;
    std::vector<Perforation> perforations;
};

#endif // WELLDATA_H
