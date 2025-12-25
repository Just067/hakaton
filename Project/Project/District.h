#include "pch.h"

class District {
public:
    enum class BuildDensity { DENSE, MEDIUM, RURAL };

    District(const string& id, const string& name,
        double area_km2, BuildDensity density);

    string getId() const;
    string getName() const;
    double getArea() const;
    BuildDensity getBuildDensity() const;
    double getCoefficientK() const; // Возвращает K в зависимости от типа застройки

private:
    string district_id;
    string district_name;
    double area_km2;
    BuildDensity build_density;
};
