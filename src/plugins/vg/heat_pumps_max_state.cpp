/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
    Input(numbers).imports("actuators/heatPumps/*[number]");
    Input(maxCoolingLoads).imports("actuators/heatPumps/*[maxCoolingLoad]");
    Input(mode).imports("setpoints/heatPumps/mode[value]");
    Input(maxPowerUse).imports("setpoints/heatPumps/maxPowerUse[value]");
    Output(value).help("Running state relative to full effect").unit("[0;1]");
}

void HeatPumpsMaxState::reset() {
    _maxCoolingLoadsTotal = 0.;
    Q_ASSERT(numbers.size() == maxCoolingLoads.size());
    for (int i=0; i <numbers.size(); ++i)
        _maxCoolingLoadsTotal += numbers.at(i)*maxCoolingLoads.at(i);
    update();
}

void HeatPumpsMaxState::update() {
    value = TestNum::eqZero(mode) ? 0. : std::min(maxPowerUse/_maxCoolingLoadsTotal, 1.);
}

} //namespace

