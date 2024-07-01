/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "pad_and_fan_product.h"

using namespace base;

namespace vg {

PUBLISH(PadAndFanProduct)

PadAndFanProduct::PadAndFanProduct(QString name, Box *parent)
    : Box(name, parent)
{
    help("holds the parameters of a pad and fan product");

    Input(coolingEfficiency).equals(0.8).help("Efficiency of reaching wet-bulb temperature").unit("[0;1]");
    Input(powerUseMax).equals(4.).help("Power at `FlowRateMax`").unit("kW");
    Input(flowRateMax).equals(50.).help("Maximum air flow rate").unit("m3/s");
    Input(powerUserParasitic).equals(0.1).help("Basic power use").unit("kW");

}

} //namespace

