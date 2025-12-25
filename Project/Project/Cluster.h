#include "BaseStation.h"

class Cluster {
public:
    Cluster(const BaseStation& bs1, const BaseStation& bs2, const BaseStation& bs3);
    double calculateC() const; // C = D1^(5/2) + D2^(3/2) + D3^(1/2)

private:
    BaseStation stations[3];
};