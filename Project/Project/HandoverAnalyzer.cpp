#include "HandoverAnalyzer.h"

HandoverAnalyzer::HandoverAnalyzer(double threshold)
    : threshold(threshold) {
}

void HandoverAnalyzer::addArea(const Area& area) {
    areas.push_back(area);
}

bool HandoverAnalyzer::isAnyAreaBelowThreshold() const {
    for (const auto& area : areas) {
        if (area.isHandoverBelowThreshold(threshold)) {
            return true;
        }
    }
    return false;
}

double HandoverAnalyzer::getHandoverCoefficient() const {
    return isAnyAreaBelowThreshold() ? 1.4 : 1.0;
}