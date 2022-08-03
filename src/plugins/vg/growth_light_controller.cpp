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
    : BaseSignal(name, parent), _isOn(false)
{
    help("control lights on/off according to setting");
    Input(setting).help("Setting; 0=off, 1=sunlight-controlled, 10=on").unit("0|1|10");
    Input(lightThresholdLow).help("If controlled then light is switched on below this sunlight threshold").unit("W/m2");
    Input(lightThresholdHigh).help("If controlled then light is switched off above this sunlight threshold").unit("W/m2");
    Input(lightOutdoors).imports("outdoors[radiation]").unit("W/m2");
}

bool GrowthLightController::computeFlag() {
    if (setting == 0) {
        _isOn = false;
    }
    else if (setting == 10) {
        _isOn = true;
    }
    else if (setting == 1) {
        if (_isOn) {
            bool switchOff = (lightOutdoors > lightThresholdHigh);
            _isOn = !switchOff;
        }
        else {
            bool switchOn = (lightOutdoors < lightThresholdLow);
            _isOn = switchOn;
        }
    }
    else {
        ThrowException("Illegal setting").value(setting).hint(port("setting")->help()).context(this);
    }
    return _isOn;
}

double GrowthLightController::computeSignal(bool flag) {
    return flag ? 1. : 0.;
}

} //namespace

