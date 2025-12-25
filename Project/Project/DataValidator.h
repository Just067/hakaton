#include "District.h"
#include "Cluster.h"

class DataValidator {
public:
    static bool validateDistrict(const District& district);
    static bool validateBaseStations(const vector<BaseStation>& stations);
};