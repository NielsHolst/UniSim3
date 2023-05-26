/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "budget_layer_cover.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(BudgetLayerCover)

BudgetLayerCover::BudgetLayerCover(QString name, base::Box *parent)
    : BudgetLayer(name, parent)
{
    setClassName("vg", "BudgetLayer");
    Input(coverPerGroundArea).imports("geometry[coverPerGroundArea]");
    Input(indoorsRh).imports("indoors[rh]");
}

double BudgetLayerCover::updateCondensation() {
    // Condensation on inside of cover
    // Bottom volume is indoors
    const double
        indoorsAh = ahFromRh(*temperatureVolumeBottom, indoorsRh),
        coverSah  = sah(temperature);
    condensation = 2e-3*coverPerGroundArea*std::max(indoorsAh - coverSah, 0.);
    return condensation;
}

}
