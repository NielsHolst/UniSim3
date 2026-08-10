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
        undamaged, parasitised1, parasitised2;

    // Outputs
    double undamagedTo1, undamagedTo2, parasitised1To2;

    // Methods
    struct Poisson {
        double zero, oneOrMore;
    };
    struct TruncatedPoisson {
        double one, more;
    };

    Poisson poisson(double average);
    TruncatedPoisson truncatedPoisson(double average);
};

} //namespace
