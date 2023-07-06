/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "budget_layer.h"
#include "budget_volume.h"
#include "layer_adjusted.h"

using namespace base;
using namespace phys_math;
using TestNum::eqZero;

namespace vg {

PUBLISH(BudgetLayer)

BudgetLayer::BudgetLayer(QString name, base::Box *parent)
    : Box(name, parent)
{
    Input(initTemperature).equals(20.).unit("oC").help("Initial temperature");
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
    Output(radiationDeltaT).unit("oC").help("Change in temperature due to net radiation");
    Output(convectionDeltaT).unit("oC").help("Change in temperature due to net convection/conduction");
    Output(totalDeltaT).unit("oC").help("Total change in temperature");
    Output(condensation).unit("kg/m2").help("Condensation (top+bottom)");
    Output(netRadiation).unit("W/m2").help("Net radiation (absorption - emission");
    Output(netConvection).unit("W/m2").help("Net radiation (absorption - emission");
}

void BudgetLayer::attach(const LayerAdjusted *layer, BudgetVolume *top, BudgetVolume *bottom)
{
    // Attach layer parameters
    attachedLayer = layer;
    emissivityTop    = attachedLayer->port("lwAbsorptivityTopAdj")->valuePtr<double>();
    emissivityBottom = attachedLayer->port("lwAbsorptivityBottomAdj")->valuePtr<double>();
    UtopAdj          = attachedLayer->port("UtopAdj")->valuePtr<double>();
    UbottomAdj       = attachedLayer->port("UbottomAdj")->valuePtr<double>();
    heatCapacity     = attachedLayer->port("heatCapacityAdj")->valuePtr<double>();
    screenEffectiveArea = attachedLayer->peakPort("effectiveArea") ?
                       attachedLayer->port("effectiveArea")->valuePtr<double>() : nullptr;
    // Attach volumes
    volumeTop = top;
    volumeBottom = bottom;
    if (top)
        top->addLayer(this);
    if (bottom)
        bottom->addLayer(this);
    temperatureVolumeTop    = top    ? top   ->port("temperature")->valuePtr<double>() : nullptr;
    temperatureVolumeBottom = bottom ? bottom->port("temperature")->valuePtr<double>() : nullptr;
}

void BudgetLayer::reset() {
    temperature = initTemperature;
    lwEmissionTopUpdatedExternally    = (port("lwEmissionTop")->status()    == PortStatus::Redefined);
    lwEmissionBottomUpdatedExternally = (port("lwEmissionBottom")->status() == PortStatus::Redefined);
    convectionTopUpdatedExternally    = (port("convectionTop")->status()    == PortStatus::Redefined);
    convectionBottomUpdatedExternally = (port("convectionBottom")->status() == PortStatus::Redefined);
    if (convectionTopUpdatedExternally && !temperatureVolumeTop)
        ThrowException("Volume on top is missing").context(this);
    if (convectionBottomUpdatedExternally && !temperatureVolumeBottom)
        ThrowException("Volume below is missing").context(this);
    update();
}


void BudgetLayer::updateNetRadiation() {
    netRadiation =   swAbsorbedTop + swAbsorbedBottom
                   + lwAbsorbedTop + lwAbsorbedBottom
                   - lwEmissionTop - lwEmissionBottom;
}

double BudgetLayer::updateDeltaT(double timeStep) {
    if (eqZero(*heatCapacity)) {
        radiationDeltaT =
        convectionDeltaT =
        totalDeltaT = 0.;
    }
    else {
        // Change by radiation
        radiationDeltaT = netRadiation*timeStep/(*heatCapacity);

        // Change by convection
        const double convAbsorbed = convectionTop + convectionBottom;
        convectionDeltaT = convAbsorbed*timeStep/(*heatCapacity);

        // Change total
        totalDeltaT = radiationDeltaT + convectionDeltaT;
    }
    return totalDeltaT;
}

void BudgetLayer::updateLwEmission() {
    if (!lwEmissionTopUpdatedExternally)
        lwEmissionTop    = Sigma*(*emissivityTop)*p4K(temperature);
    if (!lwEmissionBottomUpdatedExternally)
        lwEmissionBottom = Sigma*(*emissivityBottom)*p4K(temperature);
}

void BudgetLayer::updateConvection() {
    // Flux is positive if volume is warmer than layer
    if (!convectionTopUpdatedExternally) {
        convectionTop    = eqZero(*UtopAdj) ? 0. : ((*temperatureVolumeTop)    - temperature)*(*UtopAdj);
    }
    if (!convectionBottomUpdatedExternally) {
        convectionBottom = eqZero(*UbottomAdj) ? 0. : ((*temperatureVolumeBottom) - temperature)*(*UbottomAdj);
    }
    netConvection = convectionTop + convectionBottom;

    // Transfer fluxes to neighbouring volumes
    if (volumeBottom)
        volumeBottom->addHeatInflux(-convectionBottom);
    if (volumeTop)
        volumeTop->addHeatInflux(-convectionTop);
}

double BudgetLayer::updateCondensation() {
    return 0.;
}

}

