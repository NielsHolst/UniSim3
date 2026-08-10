#pragma once
#include <base/box.h>
#include <base/nbd.h>

namespace callosobruchus {

class Survival : public base::Box
{
public:
    Survival(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    int seedCapacity;
    double eggsTotal, seedsTotal, k, exponent;
    // Outputs
    double avgEggsPerSeed, value;
    // Data
    base::Nbd _nbd;
    // Methods
    double survival(int numEggsInSeed) const;
};

} //namespace
