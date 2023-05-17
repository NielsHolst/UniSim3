/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ACTUATOR_GROWTH_LIGHT_H
#define ACTUATOR_GROWTH_LIGHT_H

#include <base/box.h>

namespace vg {

class ActuatorGrowthLight : public base::Box {
public:
    ActuatorGrowthLight(QString name, base::Box *parent);
    void reset();
    void update();
private:
    // Inputs
    bool isOn;
    double ballast, power,
        parPhotonCoef, efficiency,
        propSw, propLw, propConv,
        propBallastLw, propBallastConv;
        ;
    // Outputs
    double
        parEmissionBottom,
        swEmissionBottom,
        lwEmissionBottom, lwEmissionTop,
        convectionBottom, convectionTop,
        powerUsage;
    // Methods
    void noLight();
};
} //namespace


#endif
