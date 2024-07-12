/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
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
    Input(efficiency).help("Not used");
    Input(maxRate).equals(1.2).help("Maximum humidification rate").unit("kg/s");
    Input(maxPowerUse).equals(2.4).help("Maximum power use for fogging").unit("kW");
    Input(parasiticLoad).equals(0.1).help("Maximum basic power use").unit("kW");
}

} //namespace

