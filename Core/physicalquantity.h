#ifndef PHYSICALQUANTITY_H
#define PHYSICALQUANTITY_H

#include "unit.h"
#include <vector>
#include <string>
#include <map>

class PhysicalQuantity : public Unit {
public:
    PhysicalQuantity();
    enum class Quantity {
        WELLDATA, WELLBORE_PRESSURE, RESERVOIR_PRESSURE, LIQUID_RATE, WATER_RATE, OIL_RATE, GAS_RATE,
        CUBEDATA, INITCUBES, PERMX, PERMY, PERMZ, PORO, DX, DY, DZ, VOLUME, NTG, ACTNUM, REGIONS, FIPNUM,
        PVTNUM, ROCKNUM, SATNUM, EQLNUM, USERCUBE, CUBE,
        /*Величины PVT данных*/ PVTDATA, REF_PRESSURE, VOL_EXPANSION, COMPRESSIBILITY, VISCOSITY, VISC_GRADIENT, DENSITY
    };
    enum class DataType {
        INT,
        DOUBLE
    };
private:
    std::map<Quantity, std::string> welldata;
    std::map<Quantity, std::string> cubNameStr;
    std::map<Quantity, std::string> cubNameVar;
    std::map<Quantity, Types> quantityunit;
    std::map<Quantity, DataType> cubesdatatype;
    std::map<Quantity, std::string> pvtDataNames;
    std::map<Quantity, Types> pvtDataUnit;

    std::map<Quantity, std::string> controlQuantityNames;
public:
    std::vector<std::string> controlItems();
    Quantity controlType(const std::string& name);
    std::string controlName(Quantity type);
    std::map<Quantity, std::string> getCubesNameStr();
    std::map<Quantity, std::string> getCubesNameVar();
    std::string getCubeNameVar(Quantity type);
    std::map<Quantity, std::string> getPVTDataNames();
};

#endif // PHYSICALQUANTITY_H
