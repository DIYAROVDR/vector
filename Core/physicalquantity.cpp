#include "physicalquantity.h"

PhysicalQuantity::PhysicalQuantity() {
    welldata = {
        {Quantity::WELLBORE_PRESSURE, "Забойное давление"},
        {Quantity::RESERVOIR_PRESSURE, "Пластовое давление"},
        {Quantity::LIQUID_RATE, "Дебит жидкости"},
        {Quantity::WATER_RATE, "Дебит воды"},
        {Quantity::OIL_RATE, "Дебит нефти"},
        {Quantity::GAS_RATE, "Дебит газа"},
    };

    cubNameStr = {
        {Quantity::DX, "Размер ячейки по X"},
        {Quantity::DY, "Размер ячейки по Y"},
        {Quantity::DZ, "Размер ячейки по Z"},
        {Quantity::ACTNUM,"Активные ячейки"},
        {Quantity::NTG,"Песчанистость"},
        {Quantity::PORO, "Пористость"},
        {Quantity::VOLUME, "Объем ячейки"},
        {Quantity::PERMX, "Проницаемость по X"},
        {Quantity::PERMY, "Проницаемость по Y"},
        {Quantity::PERMZ, "Проницаемость по Z"},
        {Quantity::FIPNUM,"Регионы отчетов (пласты)"},
        {Quantity::PVTNUM,"Регионы PVT свойств флюидов"},
        {Quantity::ROCKNUM,"Регионы свойст породы"},
        {Quantity::SATNUM,"Регионы фильтрации"},
        {Quantity::EQLNUM,"Регионы равновесия"},
        {Quantity::CUBE,"Куб"}
    };

    cubNameVar = {
        {Quantity::PERMX, "PERMX"},
        {Quantity::PERMY, "PERMY"},
        {Quantity::PERMZ, "PERMZ"},
        {Quantity::PORO, "PORO"},
        {Quantity::DX, "DX"},
        {Quantity::DY, "DY"},
        {Quantity::DZ, "DZ"},
        {Quantity::VOLUME, "VOLUME"},
        {Quantity::NTG,"NTG"},
        {Quantity::ACTNUM,"ACTNUM"},
        {Quantity::FIPNUM,"FIPNUM"},
        {Quantity::PVTNUM,"PVTNUM"},
        {Quantity::ROCKNUM,"ROCKNUM"},
        {Quantity::SATNUM,"SATNUM"},
        {Quantity::EQLNUM,"EQLNUM"},
        {Quantity::CUBE,"CUBE"}
    };

    quantityunit = {
        {Quantity::WELLBORE_PRESSURE, Types::PRESSURE},
        {Quantity::RESERVOIR_PRESSURE, Types::PRESSURE},
        {Quantity::LIQUID_RATE, Types::RATE},
        {Quantity::WATER_RATE, Types::RATE},
        {Quantity::OIL_RATE, Types::RATE},
        {Quantity::GAS_RATE, Types::RATE},
        {Quantity::PERMX, Types::PERMEABILITY},
        {Quantity::PERMY, Types::PERMEABILITY},
        {Quantity::PERMZ, Types::PERMEABILITY},
        {Quantity::PORO, Types::POROSITY},
        {Quantity::DX, Types::LENGTH},
        {Quantity::DY, Types::LENGTH},
        {Quantity::DZ, Types::LENGTH},
        {Quantity::VOLUME, Types::VOLUME},
        {Quantity::NTG, Types::DIMENSIONLESS},
        {Quantity::ACTNUM, Types::DIMENSIONLESS},
        {Quantity::FIPNUM, Types::DIMENSIONLESS},
        {Quantity::PVTNUM, Types::DIMENSIONLESS},
        {Quantity::ROCKNUM, Types::DIMENSIONLESS},
        {Quantity::SATNUM, Types::DIMENSIONLESS},
        {Quantity::EQLNUM, Types::DIMENSIONLESS},
        {Quantity::CUBE, Types::UNDEFINED}
    };

    cubesdatatype = {
        {Quantity::PERMX, DataType::DOUBLE},
        {Quantity::PERMY, DataType::DOUBLE},
        {Quantity::PERMZ, DataType::DOUBLE},
        {Quantity::PORO, DataType::DOUBLE},
        {Quantity::DX, DataType::DOUBLE},
        {Quantity::DY, DataType::DOUBLE},
        {Quantity::DZ, DataType::DOUBLE},
        {Quantity::VOLUME, DataType::DOUBLE},
        {Quantity::NTG,DataType::INT},
        {Quantity::ACTNUM,DataType::INT},
        {Quantity::FIPNUM,DataType::INT},
        {Quantity::PVTNUM,DataType::INT},
        {Quantity::ROCKNUM,DataType::INT},
        {Quantity::SATNUM,DataType::INT},
        {Quantity::EQLNUM,DataType::INT},
        {Quantity::CUBE,DataType::DOUBLE}
    };
}


std::vector<std::string> PhysicalQuantity::controlItems() {
    std::vector<std::string> result;

    // Интересующие нас величины
    std::vector<Quantity> quantities = {
        Quantity::LIQUID_RATE,
        Quantity::WATER_RATE,
        Quantity::OIL_RATE,
        Quantity::GAS_RATE,
        Quantity::WELLBORE_PRESSURE
    };

    // Проходим по всем интересующим нас величинам
    for (const auto& quantity : quantities) {
        // Получаем название величины
        std::string name = welldata[quantity];

        // Получаем тип единицы измерения для данной величины
        Types unitType = quantityunit[quantity];

        // Получаем единицу измерения для выбранной системы единиц
        std::string unit = unitvalue[outsys][unitType];

        // Формируем строку "Название величины" + "единица измерения"
        std::string item = name + " (" + unit + ")";

        // Добавляем строку в результат
        result.push_back(item);

        controlQuantityNames[quantity] = item;
    }

    return result;
}


std::string PhysicalQuantity::controlName(Quantity type) {
    return controlQuantityNames[type];
}

std::string PhysicalQuantity::getCubeNameVar(Quantity type) {
    return cubNameVar[type];
}

std::map<PhysicalQuantity::Quantity, std::string> PhysicalQuantity::getCubesNameVar() {
    return cubNameVar;
}

std::map<PhysicalQuantity::Quantity, std::string> PhysicalQuantity::getCubesNameStr() {
    return cubNameStr;
}


PhysicalQuantity::Quantity PhysicalQuantity::controlType(const std::string& name) {
    for (const auto& pair : controlQuantityNames) {
        if (pair.second == name) {
            return pair.first;
        }
    }
    return Quantity::WELLBORE_PRESSURE;
}
