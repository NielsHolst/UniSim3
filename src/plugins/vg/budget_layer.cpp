/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "budget_layer.h"
#include "budget_volume.h"
#include "layer_adjusted.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(BudgetLayer)

BudgetLayer::BudgetLayer(QString name, base::Box *parent)
    : Box(name, parent)
{
    Input(initTemperature).equals(20.).unit("oC").help("Initial temperature");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Output(temperature);
    Output(swEmissionTop);
    Output(swEmissionBottom);
    Output(swFlowTop);
    Output(swFlowBottom);
    Output(swAbsorbedTop);
    Output(swAbsorbedBottom);
    Output(lwEmissionTop);
    Output(lwEmissionBottom);
    Output(lwFlowTop);
    Output(lwFlowBottom);
    Output(lwAbsorbedTop);
    Output(lwAbsorbedBottom);
    Output(parEmissionTop);
    Output(parEmissionBottom);
    Output(parFlowTop);
    Output(parFlowBottom);
    Output(parAbsorbedTop);
    Output(parAbsorbedBottom);
    Output(convectionTop);
    Output(convectionBottom);
    Output(deltaTemperature).unit("oC").help("Change in temperature");
}

void BudgetLayer::attach(const LayerAdjusted *layer, BudgetVolume *top, BudgetVolume *bottom)
{
    // Attach layer parameters
    attachedLayer = layer;
    emissivityTop    = &attachedLayer->lwAbsorptivityTopAdj;
    emissivityBottom = &attachedLayer->lwAbsorptivityBottomAdj;
    heatCapacity     = &attachedLayer->heatCapacityAdj;
    // Attach volumes
    volumeTop = top;
    volumeBottom = bottom;
    if (top)
        top->addLayer(this);
    if (bottom)
        bottom->addLayer(this);
}

void BudgetLayer::reset() {
    temperature = initTemperature;
    lwEmissionTopUpdatedExternally    = (port("lwEmissionTop")->status()    == PortStatus::Redefined);
    lwEmissionBottomUpdatedExternally = (port("lwEmissionBottom")->status() == PortStatus::Redefined);
    convectionTopUpdatedExternally    = (port("convectionTop")->status()    == PortStatus::Redefined);
    convectionBottomUpdatedExternally = (port("convectionBottom")->status() == PortStatus::Redefined);
    update();
}

void BudgetLayer::update() {
    updateLwEmission();
}

double BudgetLayer::deltaT() {
    if (*heatCapacity > 0.) {
        const double netAbsorbed = swAbsorbedTop + swAbsorbedBottom +
                lwAbsorbedTop + lwAbsorbedBottom -
                lwEmissionTop - lwEmissionBottom +
                convectionTop + convectionBottom;
        return deltaTemperature = netAbsorbed*timeStep/(*heatCapacity);
    }
    else {
        return deltaTemperature = 0.;
    }
}

void BudgetLayer::updateLwEmission() {
    if (!lwEmissionTopUpdatedExternally)
        lwEmissionTop    = Sigma*(*emissivityTop)*p4K(temperature);
    if (!lwEmissionBottomUpdatedExternally)
        lwEmissionBottom = Sigma*(*emissivityBottom)*p4K(temperature);
}

}

