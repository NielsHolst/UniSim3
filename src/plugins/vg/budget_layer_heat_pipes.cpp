/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "budget_layer_heat_pipes.h"

using namespace base;

namespace vg {

PUBLISH(BudgetLayerHeatPipes)

BudgetLayerHeatPipes::BudgetLayerHeatPipes(QString name, base::Box *parent)
    : BudgetLayer(name, parent)
{
    setClassName("vg", "BudgetLayer");
    // Radiation and heat are determined by the heat pipe actuators
    port("lwEmissionTop")   ->imports("actuators/heatPipes[lwEmissionTop]");
    port("lwEmissionBottom")->imports("actuators/heatPipes[lwEmissionBottom]");
    port("convectionTop")   ->imports("actuators/heatPipes[convectionTop]");
    port("convectionBottom")->imports("actuators/heatPipes[convectionBottom]");
    port("temperature")     ->imports("actuators/heatPipes[inflowTemperatureAvg]");
}


void BudgetLayerHeatPipes::updateLwEmission() {
    port("lwEmissionTop")->evaluate();
    port("lwEmissionBottom")->evaluate();
}

void BudgetLayerHeatPipes::updateConvection() {
    port("convectionTop")->evaluate();
    port("convectionBottom")->evaluate();
    transferConvectionToVolumes();
}


}

