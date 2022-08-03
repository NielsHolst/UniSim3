#ifndef SINGLE_BIOMASS_H
#define SINGLE_BIOMASS_H
#include <base/box.h>

namespace cowpea {

class SingleBiomass : public base::Box
{
public:
    SingleBiomass(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double initial, expectedBiomass;
    // Output
    double single;
};

}

#endif
