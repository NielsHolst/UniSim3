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
    Input(setting).help("Setting; 0=off, 1=sunlight-controlled, 10=on").unit("0|1|10");
    Input(lightThresholdLow).help("If controlled then light is switched on below this sunlight threshold").unit("W/m2");
    Input(lightThresholdHigh).help("If controlled then light is switched off above this sunlight threshold").unit("W/m2");
    Input(minPeriodOn).help("Minimum period that light stays on").unit("min");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Input(outdoorsLight).imports("outdoors[radiation]").unit("W/m2");
    Output(isOn).unit("bool").help("Is light on?");
    Output(periodOn).help("Time since last time light went on").unit("min");
}

void GrowthLightController::reset() {
    isOn = false;
}

void GrowthLightController::update() {
    // Switch light or off according to settingh and outdoors light
    bool nowOn = isOn;
    if (setting == 0) {
        isOn = false;
    }
    else if (setting == 10) {
        isOn = true;
    }
    else if (setting == 1) {
        if (isOn) {
            bool switchOff = (outdoorsLight > lightThresholdHigh);
            isOn = !switchOff;
        }
        else {
            bool switchOn = (outdoorsLight < lightThresholdLow);
            isOn = switchOn;
        }
    }
    else {
        ThrowException("Illegal setting").value(setting).hint(port("setting")->help()).context(this);
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

