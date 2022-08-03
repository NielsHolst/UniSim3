#ifndef BIOMASS_H
#define BIOMASS_H
#include <base/box.h>

namespace cowpea {

class Biomass : public base::Box
{
public:
    Biomass(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double LUE, LightIntercepted, duration, BiomassIncrementPrevious, startDate, endDate, dayOfYear;
    // Output
    double BiomassIncrementCurrent, value;
};

}

#endif
