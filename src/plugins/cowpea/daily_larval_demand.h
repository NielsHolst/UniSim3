#ifndef DAILYLARVALDEMAND_H
#define DAILYLARVALDEMAND_H
#include <base/box.h>

namespace cowpea {

class DailyLarvalDemand : public base::Box
{
public:
    DailyLarvalDemand(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double demandPerCapita, timeStep, larvalDensity;
    // Output
    double value;
};

}

#endif
