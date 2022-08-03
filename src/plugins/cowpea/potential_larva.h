#ifndef POTENTIALLARVA_H
#define POTENTIALLARVA_H
#include <base/box.h>

namespace cowpea {

class PotentialLarva : public base::Box
{
public:
    PotentialLarva(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Input
    double supplyOfFlowers, flowersPerLarva;
    // Output
    double value;
};

}

#endif
