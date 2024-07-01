/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "heat_pumps.h"

using namespace base;
using namespace vector_op;

namespace vg {

PUBLISH(HeatPumps)

HeatPumps::HeatPumps(QString name, Box *parent)
    : Box(name, parent)
{
    help("sums heat pump outputs");
    Input(powerUsePumps      ).imports("./*[powerUse]");
    Input(coolingPumps       ).imports("./*[cooling]");
    Input(energyToBufferPumps).imports("./*[energyToBuffer]");
    Input(condensationPumps  ).imports("./*[condensation]");
    Output(powerUse).help("Power use (total)").unit("W/m2");
    Output(cooling).help("Cooling effect >= 0. An equal amount of heat is available for the `HeatBuffer` if `sendToBuffer` is set").unit("W/m2");
    Output(energyToBuffer).help("Cooling energy >= 0, sent to heat buffer").unit("W/m2");
    Output(condensation).help("Rate of water condensed in the unit").unit("kg/m2/s");
}

void HeatPumps::reset() {
    update();
}

void HeatPumps::update() {
    powerUse       = sum(powerUsePumps);
    cooling        = sum(coolingPumps);
    energyToBuffer = sum(energyToBufferPumps);
    condensation   = sum(condensationPumps);
}

} //namespace

