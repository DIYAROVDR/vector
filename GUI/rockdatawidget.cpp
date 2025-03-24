#include "rockdatawidget.h"


RockDataWidget::RockDataWidget(QWidget *parent) : QWidget(parent) {
    mainFont.setPointSize(10);
    setFont(mainFont);

    units = {UnitType::PRESSURE,
             UnitType::COMPRESSIBILITY,
             UnitType::COMPRESSIBILITY,
             UnitType::COMPRESSIBILITY,
             UnitType::POROSITY,
             UnitType::POISSON_RATIO};

    /*
    double pref = 200E+5;   // опорное давление
    double cpp = 1.0E-10;   // сжимаемость породы
    double cr = 0.0;        // сжимаемость скелета породы
    double cbc = 0.0;       // сжимаемость блока (блока, содержащего смесь)
    double fi0 = 0.33;      // значение пористости при опорном давлении
    double nu0 = 0.0;       // значение коэфициента Пуассона
    */

    headers << "Регионы"
            << "Опорное давление"
            << "Сжимаемость породы"
            << "Сжимаемость скелета породы"
            << "Cжимаемость блока"
            << "Пористсоть при опорном давлении"
            << "Коэффициента Пуассона при опорном давлении";

}

void RockDataWidget::openProject() {

}

RockDataWidget::~RockDataWidget() {

}

void RockDataWidget::setRegionCount(int count) {

}





void RockDataWidget::on_btn_apply_clicked() {

}

