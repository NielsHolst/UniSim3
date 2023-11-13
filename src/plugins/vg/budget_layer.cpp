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
#include "layer.h"

using namespace base;
using namespace phys_math;
using TestNum::eqZero;

namespace vg {

PUBLISH(BudgetLayer)

BudgetLayer::BudgetLayer(QString name, base::Box *parent)
    : Box(name, parent)
{
    doWriteOnCommand(false);
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
    Output(totalDeltaTEnergy).unit("J/m2").help("Change in thermal heat contained in layer");
    Output(condensation).unit("kg/m2").help("Condensation (top+bottom)");
    Output(netRadiation).unit("W/m2").help("Net radiation (absorption - emission");
    Output(netConvection).unit("W/m2").help("Net radiation (absorption - emission");
}

void BudgetLayer::attach(const Layer *layer, BudgetVolume *top, BudgetVolume *bottom)
{
    // Attach layer parameters
    attachedLayer = layer;
    emissivityTop    = layer->port("lwAbsorptivityTop")->valuePtr<double>();
    emissivityBottom = layer->port("lwAbsorptivityBottom")->valuePtr<double>();
    Utop             = layer->port("Utop")->valuePtr<double>();
    Ubottom          = layer->port("Ubottom")->valuePtr<double>();
    heatCapacity     = layer->port("heatCapacity")->valuePtr<double>();
    state            = layer->peakPort("state") ?
                       layer->port("state")->valuePtr<double>() : nullptr;
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
    lwEmissionTopUpdatedExternally    = (port("lwEmissionTop")->status()    == PortStatus::UserDefined);
    lwEmissionBottomUpdatedExternally = (port("lwEmissionBottom")->status() == PortStatus::UserDefined);
    convectionTopUpdatedExternally    = (port("convectionTop")->status()    == PortStatus::UserDefined);
    convectionBottomUpdatedExternally = (port("convectionBottom")->status() == PortStatus::UserDefined);
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
    updateDeltaTEnergy();
    return totalDeltaT;
}

void BudgetLayer::updateDeltaTEnergy() {
    totalDeltaTEnergy = totalDeltaT*(*heatCapacity);
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
        convectionTop    = eqZero(*heatCapacity) ? 0. : ((*temperatureVolumeTop)    - temperature)*(*Utop);
    }
    if (!convectionBottomUpdatedExternally) {
        convectionBottom = eqZero(*heatCapacity) ? 0. : ((*temperatureVolumeBottom) - temperature)*(*Ubottom);
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

void BudgetLayer::checkParameters() const {
    attachedLayer->checkParameters();
}

}

