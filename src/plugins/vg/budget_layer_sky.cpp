/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "budget_layer_sky.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(BudgetLayerSky)

BudgetLayerSky::BudgetLayerSky(QString name, base::Box *parent)
    : BudgetLayer(name, parent)
{
    setClassName("vg", "BudgetLayer");
}

void BudgetLayerSky::updateLwEmission() {
    lwEmissionTop    = 0.;
    lwEmissionBottom = Sigma*(*emissivityTop)*p4K(temperature);
}

}

