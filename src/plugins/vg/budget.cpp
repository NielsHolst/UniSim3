/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include "average_cover.h"
#include "average_screen.h"
#include "budget.h"
#include "budget_layer.h"
#include "budget_volume.h"
#include "floor.h"
#include "layer_adjusted.h"
#include "plant.h"
#include "sky.h"

using namespace base;

namespace vg {

PUBLISH(Budget)

Budget::Budget(QString name, base::Box *parent)
    : Box(name, parent)
{
    help("resolves energy and water budgets across layers and volumes");
    Input(precision).equals(1e-3).unit("W/m2|mumol/m2/s").help("Precision of numerical solution");
    Output(iterations).unit("int").help("Number of iterations taken to reach solution");
}

void Budget::amend() {


}

void Budget::addVolumes() {
    // Add volumes as children
    BoxBuilder builder(this);
    builder.
        box("BudgetVolumeExternal").name("outdoors").
            port("initTemperature").imports("outdoors[temperature]").
            port("temperature").imports("outdoors[temperature]").
            port("initRh").imports("outdoors[rh]").
            port("rh").imports("outdoors[rh]").
        endbox().
        box("BudgetVolumeInternal").name("indoors").
            port("initTemperature").equals(20.).
            port("initRh").equals(70).
        endbox().
        box("BudgetVolumeExternal").name("soil").
            port("initTemperature").imports("soilTemperature[value]").
            port("temperature").imports("soilTemperature[value]").
        endbox();

    // Find volumes
    outdoorsVol = findOne<BudgetVolume*>("./outdoors");
    indoorsVol  = findOne<BudgetVolume*>("./indoors");
    soilVol     = findOne<BudgetVolume*>("./soil");

}

void Budget::addLayers() {
    // Find layers
    sky       = findOne<Sky*>("/sky");
    cover     = findOne<AverageCover*>("shelter/layers/cover");
    screens   = findMany<AverageScreen*>("shelter/layers/screens/*");
    lights    = findMany<Box*>("actuators/growthLights/*");
    heatPipes = findMany<Box*>("actuators/heatPipes/*");
    plant     = findMaybeOne<Plant*>("/plant");
    floor     = findOne<Floor*>("/floor");

    // Build layers as children
    BoxBuilder builder(this);
    builder.
        box("BudgetLayerSky").name("sky").
            port("temperature").imports("/sky[temperature]").
            port("swEmissionBottom").imports("outdoors[radiation]").
            port("parEmissionBottom").imports("outdoors[par]").
        endbox().
        box("BudgetLayer").name("cover").
        endbox();
    QStringList screenNames;
    for (AverageScreen *screen : screens) {
        builder.
            box("BudgetLayer").name(screen->objectName()).
            endbox();
        screenNames << screen->objectName();
    }
    if (lights.size() > 0) {
        builder.
            box("BudgetLayer").name("growthLights").
                port("swEmissionBottom").imports("actuators/growthLights[swEmissionBottom]").
                port("lwEmissionTop").imports("actuators/growthLights[lwEmissionTop]").
                port("lwEmissionBottom").imports("actuators/growthLights[lwEmissionBottom]").
                port("convectionTop").imports("actuators/growthLights[lwEmissionTop]").
                port("convectionBottom").imports("actuators/growthLights[lwEmissionBottom]").
                port("parEmissionBottom").imports("actuators/growthLights[parEmissionBottom]").
            endbox();
    }
    if (heatPipes.size() > 0) {
        builder.
            box("BudgetLayer").name("heatPipes").
                port("lwEmissionTop")   .imports("actuators/heatPipes[lwEmissionTop]").
                port("lwEmissionBottom").imports("actuators/heatPipes[lwEmissionBottom]").
                port("convectiveFluxTop")   .imports("actuators/heatPipes[convectiveFluxBottom]").
                port("convectiveFluxBottom").imports("actuators/heatPipes[convectiveFluxBottom]").
            endbox();
    }
    if (plant) {
        builder.
            box("BudgetLayer").name("plant").
                port("initTemperature").imports("/plant[temperature]").
                port("temperature").imports("/plant[temperature]").
            endbox();
    }
    builder.
        box("BudgetLayerFloor").name("floor").
        endbox();


    // Attach sky
    BudgetLayer *budgetLayerSky = findOne<BudgetLayer*>("./sky");
    budgetLayerSky->attach(sky, nullptr, nullptr);
    layers << budgetLayerSky;

    // Attach cover
    BudgetLayer *budgetLayerCover = findOne<BudgetLayer*>("./cover");
    budgetLayerCover->attach(cover, outdoorsVol, indoorsVol);
    layers << budgetLayerCover;

    // Attach screens
    int i = 0;
    for (const QString &screenName : screenNames) {
        BudgetLayer *budgetLayerScreen = findOne<BudgetLayer*>("./"+screenName);
        budgetLayerScreen->attach(screens[i++], indoorsVol, indoorsVol);
        layers << budgetLayerScreen;
    }

    // Attach plant
    BudgetLayer *budgetLayerPlant = findOne<BudgetLayer*>("./plant");
    budgetLayerPlant->attach(plant, indoorsVol, indoorsVol);
    layers << budgetLayerPlant;

    // Attach floor
    BudgetLayer *budgetLayerFloor = findOne<BudgetLayer*>("./floor");
    budgetLayerFloor->attach(floor, indoorsVol, soilVol);
    layers << budgetLayerFloor;

    // Done adding layers
    numLayers = static_cast<int>(layers.size());
}

void Budget::addState() {
    // Set pointers to states
    for (BudgetLayer *layer : layers) {
        swState.E  << &layer->swEmissionBottom;
        swState.E_ << &layer->swEmissionTop;
        swState.F  << &layer->swFlowBottom;
        swState.F_ << &layer->swFlowTop;
        swState.A  << &layer->swAbsorbedBottom;
        swState.A_ << &layer->swAbsorbedTop;

        lwState.E  << &layer->lwEmissionBottom;
        lwState.E_ << &layer->lwEmissionTop;
        lwState.F  << &layer->lwFlowBottom;
        lwState.F_ << &layer->lwFlowTop;
        lwState.A  << &layer->lwAbsorbedBottom;
        lwState.A_ << &layer->lwAbsorbedTop;

        parState.E  << &layer->parEmissionBottom;
        parState.E_ << &layer->parEmissionTop;
        parState.F  << &layer->parFlowBottom;
        parState.F_ << &layer->parFlowTop;
        parState.A  << &layer->parAbsorbedBottom;
        parState.A_ << &layer->parAbsorbedTop;
    }
}

void Budget::addParameters() {
    // Set pointers to parameters
    for (BudgetLayer *layer : layers) {
        swParam.a   << &layer->attachedLayer->swAbsorptivityTopAdj;
        swParam.a_  << &layer->attachedLayer->swAbsorptivityBottomAdj;
        swParam.r   << &layer->attachedLayer->swReflectivityTopAdj;
        swParam.r_  << &layer->attachedLayer->swReflectivityBottomAdj;
        swParam.t   << &layer->attachedLayer->swTransmissivityTopAdj;
        swParam.t_  << &layer->attachedLayer->swTransmissivityBottomAdj;

        lwParam.a   << &layer->attachedLayer->lwAbsorptivityTopAdj;
        lwParam.a_  << &layer->attachedLayer->lwAbsorptivityBottomAdj;
        lwParam.r   << &layer->attachedLayer->lwReflectivityTopAdj;
        lwParam.r_  << &layer->attachedLayer->lwReflectivityBottomAdj;
        lwParam.t   << &layer->attachedLayer->lwTransmissivityTopAdj;
        lwParam.t_  << &layer->attachedLayer->lwTransmissivityBottomAdj;
    }
}

void Budget::reset() {
    deltaT.fill(0., numLayers);
    update();
}

void Budget::update() {
    updateLwEmission();
    swState.init();
    lwState.init();
    parState.init();
    distributeRadiation(parState, swParam);
    distributeRadiation(swState,  swParam);
    distributeRadiation(lwState,  lwParam);
    updateDeltaT();
}

void Budget::updateLwEmission() {
    for (BudgetLayer *layer : layers)
        layer->updateLwEmission();
}

void Budget::State::init() {
    int n = A.size();
    for (int i=0; i<n; ++i) {
        *A[i]  = 0.;
        *A_[i] = 0.;
        *F[i]  = *E.at(i);
        *F_[i] = *E_.at(i);
    }
}

void Budget::distributeRadDown(State &s, const Parameters &p) {
    for (int i=0; i<numLayers-1; ++i) {
        int j = i+1;
        const double
            &cur_a_(*p.a_.at(i)),
            &cur_r_(*p.r_.at(i)),
            &cur_t_(*p.t_.at(i)),
            &nxt_a (*p.a.at(j)),
            &nxt_r (*p.r.at(j)),
            &nxt_t (*p.t.at(j)),
            &cur_F (*s.F[i]);
        double
            &cur_A_(*s.A_[i]),
            &cur_F_(*s.F_[i]),
            &nxt_F (*s.F[j]),
            &nxt_A (*s.A[j]);
        // Correct absorption and transmission of this layer and layer below
        // for reflections ad infinitum between the two layers
        double
            k = 1. - nxt_r*cur_r_,
            ah  = nxt_a/k,
            th  = nxt_t/k,
            ah_ = cur_a_/k * nxt_r,
            th_ = cur_t_/k * nxt_r,
            // Absorbed by layer below
            absorbed     = ah  * cur_F,
            // Transmitted through layer below
            transmitted  = th  * cur_F,
            // Absorbed by this layer by reflection from below
            absorbed_    = ah_ * cur_F,
            // Transmitted through this layer by reflection from below
            transmitted_ = th_ * cur_F;
        // Update layer below
        nxt_A += absorbed;
        nxt_F += transmitted;
        // Update this layer
        cur_A_ += absorbed_;
        cur_F_ += transmitted_;
        // Downwards flow from this layer has been spent
        *s.F[i] = 0;
    }
}

void Budget::distributeRadUp(State &s, const Parameters &p) {
    for (int i=numLayers-1; i>0; --i) {
        int j = i-1;
        const double
            &cur_a(*p.a.at(i)),
            &cur_r(*p.r.at(i)),
            &cur_t(*p.t.at(i)),
            &nxt_a_(*p.a_.at(j)),
            &nxt_r_(*p.r_.at(j)),
            &nxt_t_(*p.t_.at(j)),
            &cur_F_(*s.F_[i]);
        double
            &cur_A (*s.A[i]),
            &cur_F (*s.F[i]),
            &nxt_F_(*s.F_[j]),
            &nxt_A_(*s.A_[j]);
        // Correct absorption and transmission of this layer and layer above
        // for reflections ad infinitum between the two layers
        double
            k = 1. - nxt_r_*cur_r,
            ah_ = nxt_a_/k,
            th_ = nxt_t_/k,
            ah  = cur_a/k * nxt_r_,
            th  = cur_t/k * nxt_r_,
            // Absorbed by layer above
            absorbed_    = ah_ * cur_F_,
            // Transmitted through layer above
            transmitted_ = th_ * cur_F_,
            // Absorbed by this layer by reflection from above
            absorbed     = ah  * cur_F_,
            // Transmitted through this layer by reflection from above
            transmitted  = th  * cur_F_;
        // Update layer above
        nxt_A_ += absorbed_;
        nxt_F_ += transmitted_;
        // Update this layer
        cur_A   += absorbed;
        cur_F   += transmitted;
        // Upwards flow from this layer has been spent
        *s.F_[i] = 0;
    }
}

void Budget::distributeRadiation(State &s, const Parameters &p) {
    iterations = 0;
    double residual;
    do {
        ++iterations;
        distributeRadDown(s, p);
        distributeRadUp(s, p);
        residual = 0.;
        for (int i=0; i<numLayers; ++i)
            residual += *s.F.at(i) + *s.F_.at(i);
    } while (residual > precision);
}

void Budget::updateDeltaT() {
    int i = 0;
    for (BudgetLayer *layer : layers)
        deltaT[i++] = layer->deltaT();
}

}
