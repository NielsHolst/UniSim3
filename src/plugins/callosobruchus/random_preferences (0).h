#pragma once
#include <boost/random/uniform_01.hpp>
#include <boost/random.hpp>
#include <base/box.h>

namespace callosobruchus {

class RandomPreferences : public base::Box
{
public:
    RandomPreferences(QString name, Box *parent);
    void reset();
private:
    // Outputs
    double p0, p1, p2;
    // Data
    boost::uniform_01<double> _distribution;
    boost::random::mt19937 _generator;
    // Methods
    double u();
};

} //namespace
