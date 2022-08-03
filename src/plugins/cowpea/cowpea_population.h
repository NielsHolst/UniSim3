#ifndef COWPEA_POPULATION_H
#define COWPEA_POPULATION_H
#include <base/box.h>

namespace cowpea {

class CowpeaPopulation : public base::Box
{
public:
    CowpeaPopulation(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double initial, seedsSize, established;
    // Output
    double density;
};

}

#endif
