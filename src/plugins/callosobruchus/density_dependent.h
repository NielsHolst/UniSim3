#pragma once
#include <base/box.h>
#include <base/nbd.h>

namespace callosobruchus {

class DensityDependent : public base::Box
{
public:
    DensityDependent(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    int seedCapacity;
    double beetlesProducedTotal, seedsTotal, k, exponent;
    // Outputs
    double avgBeetlesProducedPerSeed, survival, damage;
    // Data
    base::Nbd _nbd;
    // Methods
    double surv(int beetlesProducedTotalInSeed) const;
};

} //namespace
