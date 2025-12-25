#include "pch.h"
#include "BSCalculatorService.h"


void printResult(const BSCalculatorService::CalculationResult& result, const string& districtName) {
    cout << "\n=== Результаты для района '" << districtName << "' ===" << endl;
    cout << fixed << setprecision(4);
    cout << "Средний радиус (R_avg): " << result.R_avg << " км" << endl;
    cout << "Количество сот (L): " << result.L << endl;
    cout << "Кластерность (C): " << result.C << endl;
    cout << "Базовое количество БС (n): " << result.n_before_correction << endl;
    cout << "Поправка по хэндоверу: " << (result.handover_correction_applied ? "ПРИМЕНЕНА (×1.4)" : "не применена") << endl;
    cout << "Количество после поправки: " << result.n_after_correction << endl;
    cout << "Окончательное количество БС (округлено вверх): " << result.final_bs_count << endl;
    cout << "=========================================\n" << endl;
}

int main() {
    SetConsoleOutputCP(1251);
    try {
        cout << "=== ТЕСТ 1: Плотная застройка без поправки по хэндоверу ===" << endl;
        cout << "Ожидаемый результат: 1 БС\n" << endl;

        // 1. Создаем район "Центральный"
        District district(
            "D001",                    // district_id
            "Центральный",             // district_name
            12.5,                      // area_s_km2 (площадь района)
            District::BuildDensity::DENSE  // тип застройки: плотная
        );

        // 2. Создаем базовые станции согласно тесту
        vector<BaseStation> stations = {
            // БС1: S = 3.14 км², handover_avg = 45, handover_limit = 40
            BaseStation("BS001", 3.14, 45.0, 40.0, "900 MHz"),

            // БС2: S = 12.57 км², handover_avg = 48, handover_limit = 40
            BaseStation("BS002", 12.57, 48.0, 40.0, "1800 MHz"),

            // БС3: S = 28.27 км², handover_avg = 52, handover_limit = 40
            BaseStation("BS003", 28.27, 52.0, 40.0, "2100 MHz")
        };

        // 3. Валидация данных
        cout << "Валидация данных..." << endl;

        if (!DataValidator::validateDistrict(district)) {
            cerr << "Ошибка валидации района!" << endl;
            return 1;
        }

        if (!DataValidator::validateBaseStations(stations)) {
            cerr << "Ошибка валидации базовых станций!" << endl;
            return 1;
        }

        // Создаем кластер для проверки
        Cluster cluster(stations[0], stations[1], stations[2]);

        // 4. Вывод исходных данных
        cout << "Исходные данные:" << endl;
        cout << "- Район: " << district.getName()
            << " (площадь: " << district.getArea() << " км², тип: плотная)" << endl;
        cout << "- Коэффициент K: " << district.getCoefficientK() << endl;

        cout << "\nБазовая станции:" << endl;
        for (const auto& station : stations) {
            cout << "  " << station.getId()
                << ": S=" << station.getCoverageArea() << " км², "
                << "R=" << fixed << setprecision(1) << station.calculateRadius() << " км, "
                << "handover=" << station.getHandoverAvg() << "/" << station.getHandoverLimit() << endl;
        }

        // 5. Расчет
        cout << "\nВыполнение расчета..." << endl;

        // R0 = 1.0 км (по умолчанию), порог хэндовера = 40.0
        auto result = BSCalculatorService::calculateForDistrict(district, stations, 1.0, 40.0);

        // 6. Вывод результатов
        printResult(result, district.getName());

        // 7. Проверка ожидаемого результата
        if (result.final_bs_count == 1) {
            cout << " ТЕСТ ПРОЙДЕН: Получено ожидаемое значение - 1 БС" << endl;
        }
        else {
            cout << " ТЕСТ НЕ ПРОЙДЕН: Ожидалось 1 БС, получено " << result.final_bs_count << " БС" << endl;
        }

        // 8. Дополнительно: проверка промежуточных значений
        cout << "\nПроверка промежуточных значений:" << endl;

        // Расчет радиуса каждой БС
        cout << "Радиусы БС:" << endl;
        for (const auto& station : stations) {
            cout << "  " << station.getId() << ": R = sqrt("
                << station.getCoverageArea() << "/pi) = "
                << fixed << setprecision(1) << station.calculateRadius() << " км" << endl;
        }

        // Средний радиус
        double sum_radius = 0;
        for (const auto& station : stations) {
            sum_radius += station.calculateRadius();
        }
        double avg_radius = sum_radius / stations.size();
        cout << "Средний радиус: (" << stations[0].calculateRadius() << " + "
            << stations[1].calculateRadius() << " + " << stations[2].calculateRadius()
            << ") / 3 = " << avg_radius << " км" << endl;

        // Расчет L
        double K = district.getCoefficientK();
        double R0 = 1.0;
        double L_calculated = K * pow(R0 / avg_radius, 2);
        cout << "L = K * (R0/R_avg)² = " << K << " * (" << R0 << "/" << avg_radius
            << ")^2 = " << fixed << setprecision(4) << L_calculated << endl;

        // Расчет C
        double D1 = 2 * stations[2].calculateRadius(); // Наибольший радиус у BS003
        double D2 = 2 * stations[1].calculateRadius();
        double D3 = 2 * stations[0].calculateRadius();

        double C_calculated = pow(D1, 2.5) + pow(D2, 1.5) + pow(D3, 0.5);
        cout << "C = D1^(5/2) + D2^(3/2) + D3^(1/2) = "
            << D1 << "^(2.5) + " << D2 << "^(1.5) + " << D3 << "^(0.5) = "
            << fixed << setprecision(3) << C_calculated << endl;

        // Проверка хэндовера
        cout << "\nПроверка хэндовера:" << endl;
        bool handover_correction_needed = false;
        for (const auto& station : stations) {
            bool below_limit = station.getHandoverAvg() < station.getHandoverLimit();
            cout << "  " << station.getId() << ": " << station.getHandoverAvg()
                << (below_limit ? " < " : " >= ")
                << station.getHandoverLimit() << " -> "
                << (below_limit ? "требуется поправка" : "поправка не требуется") << endl;
            if (below_limit) {
                handover_correction_needed = true;
            }
        }
        cout << "Итог: поправка " << (handover_correction_needed ? "применена" : "не применена") << endl;

    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    // 9. Пример расчета для нескольких районов
    cout << "\n\n=== РАСЧЕТ ДЛЯ НЕСКОЛЬКИХ РАЙОНОВ ===" << endl;

    try {
        // Создаем несколько районов для суммарного расчета
        vector<BSCalculatorService::CalculationResult> allResults;

        // Район А (из теста 1)
        District districtA("D001", "Центральный", 12.5, District::BuildDensity::DENSE);
        vector<BaseStation> stationsA = {
            BaseStation("BS001", 3.14, 45.0, 40.0),
            BaseStation("BS002", 12.57, 48.0, 40.0),
            BaseStation("BS003", 28.27, 52.0, 40.0)
        };

        auto resultA = BSCalculatorService::calculateForDistrict(districtA, stationsA);
        allResults.push_back(resultA);

        // Район Б (из теста 2 - средняя застройка с поправкой)
        District districtB("D002", "Спальный", 18.0, District::BuildDensity::MEDIUM);
        vector<BaseStation> stationsB = {
            BaseStation("BS004", 7.07, 35.0, 40.0),
            BaseStation("BS005", 19.63, 42.0, 40.0),
            BaseStation("BS006", 50.27, 45.0, 40.0)
        };

        auto resultB = BSCalculatorService::calculateForDistrict(districtB, stationsB);
        allResults.push_back(resultB);

        // Район В (из теста 6 - много БС)
        District districtC("D003", "Мегаполис", 2.0, District::BuildDensity::DENSE);
        vector<BaseStation> stationsC = {
            BaseStation("BS007", 0.0314, 50.0, 40.0),
            BaseStation("BS008", 0.1257, 55.0, 40.0),
            BaseStation("BS009", 0.2827, 60.0, 40.0)
        };

        auto resultC = BSCalculatorService::calculateForDistrict(districtC, stationsC);
        allResults.push_back(resultC);

        // Выводим результаты по каждому району
        cout << "\nРезультаты по районам:" << endl;
        cout << setw(15) << left << "Район"
            << setw(10) << "БС"
            << setw(15) << "Поправка"
            << endl;
        cout << string(40, '-') << endl;

        vector<string> districtNames = { "Центральный", "Спальный", "Мегаполис" };
        for (size_t i = 0; i < allResults.size(); ++i) {
            cout << setw(15) << left << districtNames[i]
                << setw(10) << allResults[i].final_bs_count
                << setw(15) << (allResults[i].handover_correction_applied ? "ДА" : "нет")
                << endl;
        }

        // Суммарный расчет
        int total = BSCalculatorService::calculateTotal(allResults);
        cout << string(40, '-') << endl;
        cout << setw(15) << left << "ИТОГО:"
            << setw(10) << total
            << setw(15) << ""
            << endl;

        // Ожидаемый результат по тесту 7: 1 + 1 + 31 = 33 БС
        cout << "\nОжидаемый результат (по тесту 7): 1 + 1 + 31 = 33 БС" << endl;
        cout << "Полученный результат: " << total << " БС" << endl;

        if (total == 33) {
            cout << "Суммарный расчет верный!" << endl;
        }
        else {
            cout << "Расхождение в расчетах!" << endl;
        }

    }
    catch (const exception& e) {
        cerr << "Ошибка при расчете нескольких районов: " << e.what() << endl;
    }

    return 0;
}