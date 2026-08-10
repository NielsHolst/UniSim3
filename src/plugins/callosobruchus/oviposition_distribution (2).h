#pragma once
#include <base/box.h>

namespace callosobruchus {

class OvipositionDistribution : public base::Box
{
public:
    OvipositionDistribution(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double numEggsLaid,
        p0, p1, p2,
        parasitised0, parasitised1, parasitised2;

    // Outputs
    double p0To1, p0To2, p1To2;

    // Methods
    struct Poisson {
        double one, more;
    };
    Poisson poisson(double average);
};

} //namespace
