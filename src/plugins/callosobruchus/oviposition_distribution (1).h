#pragma once
#include <boost/random/uniform_01.hpp>
#include <boost/random.hpp>
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
    double undamagedTo1, undamagedTo2, parasitised1To2;

    // Data
    boost::uniform_01<double> _distribution;
    boost::random::mt19937 _generator;
    double _carryOver;

    // Methods
    double u() {
        return _distribution(_generator);
    }

};

} //namespace
