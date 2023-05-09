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
#include "layer_adjusted.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(BudgetLayer)

BudgetLayer::BudgetLayer(QString name, base::Box *parent)
    : Box(name, parent),
    psw(nullptr), plw(nullptr), sw(*psw), lw(*plw),
    swS {swEmissionTop,  swEmissionBottom,  swFlowTop,  swFlowBottom,  swAbsorbedTop,  swAbsorbedBottom},
    lwS {lwEmissionTop,  lwEmissionBottom,  lwFlowTop,  lwFlowBottom,  lwAbsorbedTop,  lwAbsorbedBottom},
    parS{swEmissionTop, parEmissionBottom, parFlowTop, parFlowBottom, parAbsorbedTop, parAbsorbedBottom}
{
    Input(updateLwFromTemperature).unit("bool").equals(true).help("Update long-wave emission from temperature?");
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
    Output(convectiveFluxTop);
    Output(convectiveFluxBottom);
}

BudgetLayer::~BudgetLayer() {
    delete psw;
    delete plw;
}

void BudgetLayer::attach(const LayerAdjusted *layer)
{
    attachedLayer = layer;
    psw = new Parameters{layer->swAbsorptivityTopAdj,    layer->swReflectivityTopAdj,    layer->swTransmissivityTopAdj,
                         layer->swAbsorptivityBottomAdj, layer->swReflectivityBottomAdj, layer->swTransmissivityBottomAdj};
    plw = new Parameters{layer->lwAbsorptivityTopAdj,    layer->lwReflectivityTopAdj,    layer->lwTransmissivityTopAdj,
                         layer->lwAbsorptivityBottomAdj, layer->lwReflectivityBottomAdj, layer->lwTransmissivityBottomAdj};

}

void BudgetLayer::reset() {
    temperature = 20.;
}

void BudgetLayer::update() {
}

void BudgetLayer::updateLwEmission() {
    if (updateLwFromTemperature) {
        lwEmissionTop    = Sigma*lw.a *p4K(temperature);
        lwEmissionBottom = Sigma*lw.a_*p4K(temperature);
    }
}

void BudgetLayer::transferEmissionsToFlows() {
    swS.F   = swS.E;
    swS.F_  = swS.E_;
    lwS.F   = lwS.E;
    lwS.F_  = lwS.E_;
    parS.F  = parS.E;
    parS.F_ = parS.E_;
}

}

