#ifndef INITIAL_BIOMASS_H
#define INITIAL_BIOMASS_H
#include <base/box.h>

namespace cowpea {

class initialBiomass : public base::Box
{
public:
    initialBiomass(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double initial, biomassPerPlant;
    // Output
    double value;
};

}

#endif
