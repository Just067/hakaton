#include "Area.h"

class HandoverAnalyzer {
private:
    vector<Area> areas;
    double threshold; // пороговое значение хэндовера

public:
    HandoverAnalyzer(double threshold = 0.0);
    void addArea(const Area& area);
    bool isAnyAreaBelowThreshold() const;
    double getHandoverCoefficient() const;
};