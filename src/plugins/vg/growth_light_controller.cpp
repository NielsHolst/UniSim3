/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "growth_light_controller.h"

using namespace base;

namespace vg {
	
PUBLISH(GrowthLightController)

GrowthLightController::GrowthLightController(QString name, Box *parent)
    : Box(name, parent), isOn(false)
{
    help("control lights on/off according to setting");
    Input(mode).help("Control mode can be 0=off, 1=by threshold, 2=by light sum, 10=on").unit("0|1|10");
    Input(input).help("Current value of threshold variable");
    Input(thresholdLow). help("If threshold-controlled the light is switched on below this threshold").unit("W/m2");
    Input(thresholdHigh).help("If threshold-controlled the light is switched off above this threshold").unit("W/m2");
    Input(desiredLightSum).help("Targetted light sum").unit("mol PAR/m2/d");
    Input(minPeriodOn).help("Minimum period that light stays on").unit("min");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Output(isOn).unit("bool").help("Is light on?");
    Output(periodOn).help("Time since last time light went on").unit("min");
}

void GrowthLightController::reset() {
    isOn = false;
}

void GrowthLightController::update() {
    // Switch light or off according to mode and input
    bool
        nowOn = isOn,
        lightSumAchieved = !atMidnight && (currentLightSum + expectedSunlightSum > desiredLightSum);
    if (mode == static_cast<int>(Mode::Off)) {
        isOn = false;
    }
    else if (mode == static_cast<int>(Mode::On)) {
        isOn = true;
    }
    else if (mode == static_cast<int>(Mode::Threshold)) {
        if (isOn) {
            bool switchOff = (input > thresholdHigh);
            isOn = !switchOff;
        }
        else {
            bool switchOn = (input < thresholdLow);
            isOn = switchOn;
        }
    }
    else {
        ThrowException("Illegal setting").value(mode).hint(port("setting")->help()).context(this);
    }

    // Observe minimum on-time
    bool justedSwitchedOn   = !nowOn && isOn,
         attemptedSwitchOff = nowOn && !isOn;
    if (justedSwitchedOn) {
        periodOn = 0;
    }
    else {
        periodOn +=  timeStepSecs/60.;
        // Overrule switch-off
        if (attemptedSwitchOff && (periodOn < minPeriodOn))
            isOn = true;
    }
}

} //namespace

