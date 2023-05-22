/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "budget_layer_floor.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(BudgetLayerFloor)

BudgetLayerFloor::BudgetLayerFloor(QString name, base::Box *parent)
    : BudgetLayer(name, parent)
{
    Input(indoorsRh).imports("indoors[rh]");
}

void BudgetLayerFloor::updateLwEmission() {
    lwEmissionTop    = Sigma*(*emissivityTop)*p4K(temperature);
    lwEmissionBottom = 0.;
}

double BudgetLayerFloor::updateCondensation() {
    // Condensation on top of floor
    // Top volume is indoors
    const double
        indoorsAh = ahFromRh(*temperatureVolumeTop, indoorsRh),
        floorSah  = sah(temperature);
    condensation = 2e-3*std::max(indoorsAh - floorSah, 0.);
    return condensation;
}

}

