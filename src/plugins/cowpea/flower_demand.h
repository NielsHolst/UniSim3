#ifndef FLOWERDEMAND_H
#define FLOWERDEMAND_H
#include <base/box.h>

namespace cowpea {

class FlowerDemand : public base::Box
{
public:
    FlowerDemand(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double density, flowersPerLarva;
    // Output
    double value;
};

}

#endif
