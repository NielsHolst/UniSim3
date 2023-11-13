/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "heat_pump_product.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(HeatPumpProduct)

HeatPumpProduct::HeatPumpProduct(QString name, Box *parent)
    : Box(name, parent)
{
    help("holds the parameters of a heat pump product");
    Input(maxCoolingLoad).equals(78.4).help("Maximum cooling power").unit("kW");
    Input(coolingEfficiency).equals(0.69).help("Effective proportion of cooling power").unit("[0;1]");
    Input(maxFlowRate).equals(2.5).help("Maximum air flow rate").unit("m3/s");
    Input(parasiticLoad).equals(2.2).help("Maximum basic power use").unit("kW");
    Input(coolingTemperature).equals(10.).help("Temperature of the cooling aggregate").unit("oC");
}

} //namespace

