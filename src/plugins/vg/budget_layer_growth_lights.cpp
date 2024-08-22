/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "budget_layer_growth_lights.h"

using namespace base;

namespace vg {

PUBLISH(BudgetLayerGrowthLights)

BudgetLayerGrowthLights::BudgetLayerGrowthLights(QString name, base::Box *parent)
    : BudgetLayer(name, parent)
{
    setClassName("vg", "BudgetLayer");
    // Radiation and heat are determined by the growth light actuators
    port("parEmissionBottom")->imports("actuators/growthLights[parEmissionBottom]");
    port("swEmissionBottom") ->imports("actuators/growthLights[swEmissionBottom]");
    port("lwEmissionTop")    ->imports("actuators/growthLights[lwEmissionTop]");
    port("lwEmissionBottom") ->imports("actuators/growthLights[lwEmissionBottom]");
    port("convectionTop")    ->imports("actuators/growthLights[convectionTop]");
    port("convectionBottom") ->imports("actuators/growthLights[convectionBottom]");
}

void BudgetLayerGrowthLights::updateLwEmission() {
    port("lwEmissionTop")->evaluate();
    port("lwEmissionBottom")->evaluate();
}

void BudgetLayerGrowthLights::updateConvection() {
    port("convectionTop")->evaluate();
    port("convectionBottom")->evaluate();
    transferConvectionToVolumes();
}

}

