#include "pch.h"

class BaseStation {
public:

    BaseStation();

    BaseStation(const string& id, double coverage_area_km2,
        double handover_avg, double handover_limit,
        const string& frequency_band = "");

    string getId() const;
    double getCoverageArea() const;
    double getHandoverAvg() const;
    double getHandoverLimit() const;
    string getFrequencyBand() const;

    double calculateRadius() const; // R = √(S/π)
    double getDiameter() const;     // D = 2R

private:
    string bs_id;
    double coverage_area_km2;
    double handover_avg;
    double handover_limit;
    string frequency_band;
};
