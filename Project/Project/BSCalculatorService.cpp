#include "BSCalculatorService.h"

BSCalculatorService::CalculationResult BSCalculatorService::calculateForDistrict(
    const District& district,
    const vector<BaseStation>& stations,
    double R0,
    double handover_threshold) {

    CalculationResult result;

    // 1. Валидация входных данных
    if (!DataValidator::validateDistrict(district) ||
        !DataValidator::validateBaseStations(stations)) {
        // Возвращаем результат с ошибкой или бросаем исключение
        result.final_bs_count = 0;
        return result;
    }

    // 2. Расчет радиуса для каждой БС и среднего радиуса
    vector<double> radii;
    radii.reserve(stations.size());
    for (const auto& station : stations) {
        radii.push_back(station.calculateRadius());
    }
    result.R_avg = accumulate(radii.begin(), radii.end(), 0.0) / radii.size();

    // 3. Определение коэффициента K по типу застройки
    double K = district.getCoefficientK();

    // 4. Расчет L (количество сот)
    if (result.R_avg == 0) {
        result.L = 0;
    }
    else {
        // L = K * (R0 / R_avg)^2
        result.L = K * pow(R0 / result.R_avg, 2);
    }

    // 5. Расчет кластерности C (по первым 3 БС)
    Cluster cluster(stations[0], stations[1], stations[2]);
    result.C = cluster.calculateC();

    // 6. Расчет базового количества БС: n = L/C
    if (result.C == 0) {
        result.n_before_correction = 0;
    }
    else {
        result.n_before_correction = result.L / result.C;
    }

    // 7. Проверка условия по хэндоверу
    result.handover_correction_applied = false;
    for (const auto& station : stations) {
        if (station.getHandoverAvg() < station.getHandoverLimit()) {
            result.handover_correction_applied = true;
            break;
        }
    }

    // 8. Применение поправки при необходимости
    if (result.handover_correction_applied) {
        result.n_after_correction = result.n_before_correction * 1.4;
    }
    else {
        result.n_after_correction = result.n_before_correction;
    }

    // 9. Округление результата ВВЕРХ до целого числа
    result.final_bs_count = static_cast<int>(ceil(result.n_after_correction));

    // 10. Округляем промежуточные значения до 4 знаков после запятой
    auto roundTo4 = [](double value) {
        return round(value * 10000) / 10000;
        };

    result.R_avg = roundTo4(result.R_avg);
    result.L = roundTo4(result.L);
    result.C = roundTo4(result.C);
    result.n_before_correction = roundTo4(result.n_before_correction);
    result.n_after_correction = roundTo4(result.n_after_correction);

    return result;
}

int BSCalculatorService::calculateTotal(const vector<CalculationResult>& results) {
    int total = 0;
    for (const auto& result : results) {
        total += result.final_bs_count;
    }
    return total;
}