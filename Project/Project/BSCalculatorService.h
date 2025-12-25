#include "DataValidator.h"

class BSCalculatorService {
public:
    struct CalculationResult {
        int final_bs_count;
        double R_avg;
        double L;
        double C;
        double n_before_correction;
        double n_after_correction;
        bool handover_correction_applied;
    };

    static CalculationResult calculateForDistrict(
        const District& district,
        const vector<BaseStation>& stations,
        double R0 = 1.0,
        double handover_threshold = 40.0
    );

    static int calculateTotal(const vector<CalculationResult>& results);
};
