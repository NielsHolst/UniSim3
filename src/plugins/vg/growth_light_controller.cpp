/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
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
    Input(mode).help("Control mode can be 0=off, 1=by threshold, 2=by light sum, 10=on").unit("0|1|2|10");
    Input(input).help("Current value of threshold variable");
    Input(thresholdLow). help("If threshold-controlled the light is switched on below this threshold").unit("W/m2");
    Input(thresholdHigh).help("If threshold-controlled the light is switched off above this threshold").unit("W/m2");
    Input(desiredLightSum).help("Targetted light sum").unit("mol PAR/m2/d");
    Input(currentLightSum).help("Current light sum").unit("mol PAR/m2/d");
    Input(minPeriodOn).help("Minimum period that light stays on").unit("min");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Input(atMidnight).imports("calendar[atMidnight]");
    Output(isOn).unit("bool").help("Is light on?");
    Output(periodOn).help("Time since last time light went on").unit("min");
    Output(switchOffForToday).help("Light sum reached for today");
}

void GrowthLightController::reset() {
    isOn = false;
    switchOffForToday = false;
}

void GrowthLightController::update() {
    static QMap<int, Mode> modes = {
        {0, Mode::Off},
        {1, Mode::Threshold},
        {2, Mode::LightSum},
        {10, Mode::On}
    };
    if (!modes.contains(mode))
        ThrowException("Illegal setting").value(mode).hint(port("setting")->help()).context(this);

    // Turn light on/off according to mode
    bool nowOn = isOn;
    switch (modes.value(mode)) {
    case Mode::Off:
        isOn = false;
        switchOffForToday = false;
        break;
    case Mode::On:
        isOn = true;
        switchOffForToday = false;
        break;
    case Mode::Threshold:
        switchByThreshold();
        switchOffForToday = false;
        break;
    case Mode::LightSum:
        switchByThreshold();
        switchByLightSum();
    }

    // Obey minimum on-time
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

void GrowthLightController::switchByThreshold() {
    if (isOn) {
        bool switchOff = (input > thresholdHigh);
        isOn = !switchOff;
    }
    else {
        bool switchOn = (input < thresholdLow);
        isOn = switchOn;
    }
}

void GrowthLightController::switchByLightSum() {
    // Flag is lowered at midnight and is sticky for the rest of the day
    switchOffForToday = atMidnight ? false : (switchOffForToday || (currentLightSum > desiredLightSum));
    if (switchOffForToday)
        isOn = false;
}

} //namespace

