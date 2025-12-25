#include "DataValidator.h"

bool DataValidator::validateDistrict(const District& district) {
    // Проверка площади
    if (district.getArea() <= 0) {
        return false; // Площадь должна быть положительной
    }

    // Проверка типа застройки (уже валидно через enum)
    return true;
}

bool DataValidator::validateBaseStations(const vector<BaseStation>& stations) {
    // Проверка минимального количества БС
    if (stations.size() < 3) {
        return false; // Для расчета кластера требуется минимум 3 базовые станции
    }

    // Проверка каждой БС
    for (const auto& station : stations) {
        // Площадь покрытия должна быть положительной
        if (station.getCoverageArea() <= 0) {
            return false;
        }

        // Хэндовер не может быть отрицательным
        if (station.getHandoverAvg() < 0 || station.getHandoverLimit() < 0) {
            return false;
        }
    }

    return true;
}
