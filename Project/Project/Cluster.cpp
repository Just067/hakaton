#include "Cluster.h"

Cluster::Cluster(const BaseStation& bs1, const BaseStation& bs2, const BaseStation& bs3) {
    stations[0] = bs1;
    stations[1] = bs2;
    stations[2] = bs3;
}

double Cluster::calculateC() const {
    // Получаем диаметры
    double diameters[3] = {
        stations[0].getDiameter(),
        stations[1].getDiameter(),
        stations[2].getDiameter()
    };

    // Сортируем по убыванию
    sort(begin(diameters), end(diameters), greater<double>());

    // C = D1^(5/2) + D2^(3/2) + D3^(1/2)
    double D1 = diameters[0];
    double D2 = diameters[1];
    double D3 = diameters[2];

    return pow(D1, 2.5) + pow(D2, 1.5) + pow(D3, 0.5);
}
