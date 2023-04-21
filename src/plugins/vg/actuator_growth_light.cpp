/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "actuator_growth_light.h"
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>

using namespace base;

namespace vg {

PUBLISH(ActuatorGrowthLight)

ActuatorGrowthLight::ActuatorGrowthLight(QString name, Box *parent)
    : Box(name, parent)
{
    help("models power use and radiation from growth light");
    Input(isOn).help("Is light currently switched on?").unit("bool");
    Input(power).help("Power of installed lamps per greenhouse area").unit("W/m2");
    Input(parPhotonCoef).equals(1.6).help("Number of PAR photons per spent lamp energy").unit("micromole PAR/J");
    Input(efficiency).equals(1.).help("Proportion of intensity actually emitted").unit("[0;1]");
    Input(propLw).equals(0.1).help("Proportion of power emitted as long-wave radiation").unit("[0;1]");
    Input(ballast).help("Power used by lamp ballast").unit("W/m2");
    Output(swEmissionBottom).help("Short-wave emission downwards").unit("W/m2");
    Output(lwEmissionBottom).help("Long-wave emission downwards").unit("W/m2");
    Output(lwEmissionTop).help("Long-wave emission upwards").unit("W/m2");
    Output(parEmissionBottom).help("PAR emission downwards").unit("micromole PAR/m2/s");
    Output(powerUsage).help("Current power usage").unit("W/m2");
}

void ActuatorGrowthLight::reset() {
    noLight();
}

void ActuatorGrowthLight::update() {
    if (isOn) {
        double sw            = power*(1. - propLw),
               swDegradation = sw*(1. - efficiency),
               lw            = power*propLw + ballast + swDegradation;
        parEmissionBottom = power*parPhotonCoef*efficiency;
        swEmissionBottom  = sw - swDegradation;
        lwEmissionTop     = lw/2.;
        lwEmissionBottom  = lw/2.;
        powerUsage        = power + ballast;
    }
    else
        noLight();
}

void ActuatorGrowthLight::noLight() {
    swEmissionBottom =
    lwEmissionBottom =
    lwEmissionTop =
    parEmissionBottom =
    powerUsage = 0.;

}
} //namespace
