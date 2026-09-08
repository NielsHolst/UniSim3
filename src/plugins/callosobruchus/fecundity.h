#pragma once
#include <base/box.h>
#include <base/base_dllexport.h>

namespace callosobruchus {

class UNI_API Fecundity : public base::Box
{
public:
    Fecundity(QString name, Box *parent);
    void reset() override;
    void update() override;
private:
    // Inputs
    double sexRatio,
        temperature, slope, intercept,
        minTemperature, maxTemperature;
    // Outputs
    double value;
};

} //namespace
