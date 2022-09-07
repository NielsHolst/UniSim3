/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "growth_light.h"
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>

using namespace base;

namespace vg {

PUBLISH(GrowthLight)

GrowthLight::GrowthLight(QString name, Box *parent)
    : HeatTransferLayerBase(name, parent)
{
    help("models power use and radiation from growth light");
    Input(on).help("Is light currently switched on?").unit("bool");
    Input(ballast).help("Power used by lamp ballast").unit("W/m2");
    Input(intensity).help("Power of installed lamps per greenhouse area").unit("W/m2");
    Input(parPhotonCoef).equals(1.6).help("Number of PAR photons per spent lamp energy").unit("micromole/J");
    Input(propLw).equals(0.1).help("Proportion of power emitted as long-wave radiation").unit("[0;1]");
    Input(ageCorrectedEfficiency).equals(1.).help("Proportion of intensity actually emitted").unit("[0;1]");
    Input(minPeriodOn).help("Minimum period that light stays on").unit("m");
    port("area")->imports("construction/geometry[groundArea]",CA);
    Output(periodOn).help("Time since last time light went on").unit("m");
    Output(currentlyOn).help("Light remains on for the minimum period").unit("y|n");
    Output(powerUsage).help("Current power usage").unit("W/m2");
}

void GrowthLight::initialize() {
    swReflectivityBottom   = swReflectivityTop =
    lwReflectivityBottom   = lwReflectivityTop = 0.;
    swTransmissivityBottom = lwTransmissivityTop = 1.;
    updateAbsorptivities();
}

void GrowthLight::reset() {
    noLight();
}

void GrowthLight::update() {
    currentlyOn = on || ( currentlyOn && (periodOn < minPeriodOn) );
    if (currentlyOn) {
        periodOn += timeStep/60.;
        parEmittedDown = intensity*parPhotonCoef*ageCorrectedEfficiency;
        swEmittedDown  = intensity*(1. - propLw);
        lwFluxDown  = intensity*propLw + 0.5*ballast;
        lwFluxUp    = 0.5*ballast;
        powerUsage  = intensity + ballast;
    }
    else
        noLight();
}

void GrowthLight::noLight() {
    periodOn = 0.;
    currentlyOn = false;
    parEmittedDown = swEmittedDown = lwFluxDown = lwFluxUp = powerUsage = 0.;
}

} //namespace

