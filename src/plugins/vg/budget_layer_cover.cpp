/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
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
    Input(coverPerGroundArea).imports("gh/geometry[coverPerGroundArea]");
    Input(indoorsRh).imports("indoors[rh]");
    Output(condInsideDeltaT).unit("oC").help("Temperature change caused by inside condensation");
    Output(condOutsideDeltaT).unit("oC").help("Temperature change caused by outside condensation");
    Output(lwCoverSkyBalance).unit("W/m2 ground").help("Long-wave balance; negative means net loss");
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

void BudgetLayerCover::updateLwEmission() {
    BudgetLayer::updateLwEmission();
    // Convert W/m2 cover to W/m2 ground
    lwEmissionTop    *= coverPerGroundArea;
    lwEmissionBottom *= coverPerGroundArea;
    lwCoverSkyBalance = lwAbsorbedTop - lwEmissionTop;
}

double BudgetLayerCover::updateDeltaTByCondensation(double condensation) {
    // Add condensation (kg/m2) energy and DeltaT to values set by radiation and convection
    const double
        deltaEnergy = LHe*condensation,
        deltaT      = deltaEnergy/(*heatCapacity);
    totalDeltaTEnergy += deltaEnergy;
    totalDeltaT       += deltaT;
    return totalDeltaT;
}

}

