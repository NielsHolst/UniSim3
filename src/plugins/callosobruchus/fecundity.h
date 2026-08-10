#pragma once
#include <base/box.h>

namespace callosobruchus {

class Fecundity : public base::Box
{
public:
    Fecundity(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double sexRatio,
        temperature, slope, intercept,
        minTemperature, maxTemperature;
    // Outputs
    double value;
};

} //namespace
