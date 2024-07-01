/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
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
    QString productName;
    bool isOn;
    double
        numberInstalled, efficiency;
    // Outputs
    double
        power, ballast,
        parEmissionBottom,
        swEmissionBottom,
        lwEmissionBottom, lwEmissionTop,
        convectionBottom, convectionTop,
        powerUsage,
        minPeriodOn;
    // Data (copied from lamp product)
    double
        parPhotonCoef,
        propSw, propLw, propConv,
        propBallastLw, propBallastConv;

    // Methods
    void noLight();
    void checkProp(double prop, QString name);
};
} //namespace


#endif
