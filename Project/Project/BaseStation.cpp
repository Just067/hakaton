#include "BaseStation.h"

BaseStation::BaseStation()
    : bs_id(""), coverage_area_km2(0.0), handover_avg(0.0),
    handover_limit(0.0), frequency_band("") {
}

BaseStation::BaseStation(const string& id, double coverage_area_km2,
    double handover_avg, double handover_limit,
    const string& frequency_band)
    : bs_id(id), coverage_area_km2(coverage_area_km2),
    handover_avg(handover_avg), handover_limit(handover_limit),
    frequency_band(frequency_band) {

    if (coverage_area_km2 <= 0) {
        throw invalid_argument("Площадь покрытия должна быть положительной");
    }
    if (handover_avg < 0) {
        throw invalid_argument("Средний хэндовер не может быть отрицательным");
    }
    if (handover_limit < 0) {
        throw invalid_argument("Лимит хэндовера не может быть отрицательным");
    }
}

string BaseStation::getId() const {
    return bs_id;
}

double BaseStation::getCoverageArea() const {
    return coverage_area_km2;
}

double BaseStation::getHandoverAvg() const {
    return handover_avg;
}

double BaseStation::getHandoverLimit() const {
    return handover_limit;
}

string BaseStation::getFrequencyBand() const {
    return frequency_band;
}

double BaseStation::calculateRadius() const {
    // R = √(S/π)
    return sqrt(coverage_area_km2 / M_PI);
}

double BaseStation::getDiameter() const {
    // D = 2R
    return 2 * calculateRadius();
}