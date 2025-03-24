#include "pvdo.h"
/*
PVDO::PVDO() {}

void PVDO::setData(const std::vector<double> &data) {
    // Ожидаемая структура данных: давление, BO, µO в виде: {p1, BO1, µO1, p2, BO2, µO2, ...}
    if (data.size() % 3 != 0) {
        throw std::invalid_argument("Размер входного массива данных должен быть кратен 3");
    }

    size_t n = data.size() / 3;
    p.clear();
    p.resize(n);
    b.resize(n);
    m.resize(n);

    for (size_t i = 0; i < n; ++i) {
        p[i] = data[i * 3];
        b[i] = data[i * 3 + 1];
        m[i] = data[i * 3 + 2];
    }
}

double PVDO::B(double po) {
    if(po < p.front()){
        return MathHelper::interpolate(b,p,po);
    }
    else {
        return MathHelper::extrapolate(b,p,po);
    }
}

double PVDO::Mu(double po) {
    if(po < p.front()){
        return MathHelper::interpolate(m,p,po);
    }
    else {
        return MathHelper::extrapolate(m,p,po);
    }
}

double PVDO::Rho(double p) {
    return 0;
}
*/
