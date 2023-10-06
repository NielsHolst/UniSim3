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
    Input(productName).help("Name of product holding lamp parameters");
    Input(numberInstalled).unit("/m2").help("Number of lamps installed");
    Input(efficiency).equals(1.).help("Proportion of intensity actually emitted").unit("[0;1]");
    Input(isOn).help("Is light currently switched on?").unit("bool");

    Output(power).help("Power of installed lamps").unit("W/m2");
    Output(ballast).help("Power used by lamp ballast").unit("W/m2");
    Output(parEmissionBottom).help("PAR emission downwards").unit("micromole PAR/m2/s");
    Output(swEmissionBottom).help("Short-wave emission downwards").unit("W/m2");
    Output(lwEmissionBottom).help("Long-wave emission downwards").unit("W/m2");
    Output(lwEmissionTop).help("Long-wave emission upwards").unit("W/m2");
    Output(convectionBottom).help("Convective heat downwards").unit("W/m2");
    Output(convectionTop).   help("Convective heat upwards").unit("W/m2");
    Output(minPeriodOn).help("Minimum period that light stays on").unit("min");
    Output(powerUsage).help("Current power usage").unit("W/m2");
}

#define UPDATE_INPUT(x) x = product->port(#x)->value<double>()

void ActuatorGrowthLight::reset() {
    if (productName.toLower() == "none") {
        noLight();
        return;
    }
    Box *product = findOne<Box*>("../products/" + productName);
    UPDATE_INPUT(power)*numberInstalled;
    UPDATE_INPUT(ballast)*numberInstalled;
    UPDATE_INPUT(parPhotonCoef);
    UPDATE_INPUT(propSw);
    UPDATE_INPUT(propSw);
    UPDATE_INPUT(propLw);
    UPDATE_INPUT(propConv);
    UPDATE_INPUT(propBallastLw);
    UPDATE_INPUT(propBallastConv);
    UPDATE_INPUT(minPeriodOn);

    checkProp(propSw, "propSw");
    checkProp(propLw, "propLw");
    checkProp(propConv, "propConv");
    checkProp(propBallastLw, "propBallastLw");
    checkProp(propBallastConv, "propBallastConv");
    const double propSum = propSw + propLw + propConv;
    if (TestNum::ne(propSum, 1.0))
        ThrowException("Energy proportions sum (propSw+propLw+propConv) must be 1").value(propSum).context(this);
    const double propSum2 = propBallastLw + propBallastConv;
    if (TestNum::ne(propSum2, 1.0))
        ThrowException("Energy proportions sum (propBallastLw+propBallastConv) must be 1").value(propSum2).context(this);
    noLight();
}

void ActuatorGrowthLight::update() {
    if (isOn) {
        parEmissionBottom = parPhotonCoef*efficiency*power;
        swEmissionBottom  = propSw*power;
        lwEmissionTop     =
        lwEmissionBottom  = (propLw*power + propBallastLw*ballast)/2.;
        // Convection energy is a loss from the growth light
        convectionTop     =
        convectionBottom  = -(propConv*power + propBallastConv*ballast)/2.;
        powerUsage        = power + ballast;
    }
    else
        noLight();
}

void ActuatorGrowthLight::noLight() {
    parEmissionBottom =
    swEmissionBottom =
    lwEmissionBottom =
    lwEmissionTop =
    convectionBottom =
    convectionTop =
    powerUsage = 0.;

}

void ActuatorGrowthLight::checkProp(double prop, QString name) {
    if (prop < 0. || prop > 1.)
        ThrowException("Proportion must be inside [0;1] interval").value(name).value2(prop).context(this);
}

} //namespace

