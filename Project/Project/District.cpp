#include "District.h"

District::District(const string& id, const string& name,
    double area_km2, BuildDensity density)
    : district_id(id), district_name(name), area_km2(area_km2), build_density(density) {
}

string District::getId() const {
    return district_id;
}

string District::getName() const {
    return district_name;
}

double District::getArea() const {
    return area_km2;
}

District::BuildDensity District::getBuildDensity() const {
    return build_density;
}

double District::getCoefficientK() const {
    switch (build_density) {
    case BuildDensity::DENSE:
        return 1.21;
    case BuildDensity::MEDIUM:
        return 0.9;
    case BuildDensity::RURAL:
        return 0.47;
    default:
        return 1.0; // По умолчанию
    }
}