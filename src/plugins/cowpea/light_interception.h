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
    double LUE, LI, duration;
    // Output
    double NPP;
};

}

#endif
