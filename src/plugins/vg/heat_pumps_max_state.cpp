/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "heat_pumps_max_state.h"

using namespace base;

namespace vg {

PUBLISH(HeatPumpsMaxState)

HeatPumpsMaxState::HeatPumpsMaxState(QString name, Box *parent)
    : Box(name, parent)
{
    help("models a heat pump");
    Input(maxCoolingLoad).imports("actuators/heatPumps[maxCoolingLoad]");
    Input(maxPowerUse).imports("setpoints/heatPumps/maxPowerUse[value]");
    Input(mode).imports("setpoints/heatPumps/mode[value]");
    Output(value).help("Running state relative to full effect").unit("[0;1]");
}

void HeatPumpsMaxState::update() {
    value = TestNum::eqZero(mode) ? 0. : std::min(maxPowerUse/maxCoolingLoad, 1.);
}

} //namespace

