/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
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
    setClassName("vg", "BudgetLayer");
    Input(coverPerGroundArea).imports("gh/geometry[coverPerGroundArea]");
    Input(indoorsRh).imports("indoors[rh]");
}

double BudgetLayerScreen::updateCondensation() {
    if (state) {
        // Condensation on both sides of screen
        // Top and bottom volumes are both indoors
        const double
            indoorsAh = ahFromRh(*temperatureVolumeTop, indoorsRh),
            screenSah  = sah(temperature);
        condensation = 2.*(*state)*coverPerGroundArea*std::max(indoorsAh - screenSah, 0.);
    }
    else {
        condensation = 0.;
    }
    return condensation;
}


void BudgetLayerScreen::updateLwEmission() {
    BudgetLayer::updateLwEmission();
    // Convert W/m2 screen to W/m2 ground
    const double c = state ? (*state)*coverPerGroundArea : 0;
    lwEmissionTop    *= c;
    lwEmissionBottom *= c;
}

}

