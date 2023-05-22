/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "budget_layer_screen.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(BudgetLayerScreen)

BudgetLayerScreen::BudgetLayerScreen(QString name, base::Box *parent)
    : BudgetLayer(name, parent)
{
    Input(groundArea).imports("geometry[groundArea]");
    Input(indoorsRh).imports("indoors[rh]");
}

double BudgetLayerScreen::updateCondensation() {
    Q_ASSERT(screenEffectiveArea);
    // Condensation on both sides of screen
    // Top and bottom volumes are both indoors
    const double
        indoorsAh = ahFromRh(*temperatureVolumeTop, indoorsRh),
        screenSah  = sah(temperature);
    condensation = 2.*(*screenEffectiveArea)/groundArea*std::max(indoorsAh - screenSah, 0.);
    return condensation;
}

}

