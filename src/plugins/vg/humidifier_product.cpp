/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "humidifier_product.h"

using namespace base;

namespace vg {

PUBLISH(HumidifierProduct)

HumidifierProduct::HumidifierProduct(QString name, Box *parent)
    : Box(name, parent)
{
    help("holds humidifier product parameters");
    Input(efficiency).equals(0.8).help("Efficiency at decreasing temperature to wet-bulb temperature").unit("[0;1]");
    Input(maxRate).equals(2.).help("Maximum humidification rate").unit("kg/s");
    Input(parasiticLoad).equals(2.2).help("Maximum basic power use").unit("kW");
}

} //namespace

