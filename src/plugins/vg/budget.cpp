/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QTextStream>
#include <base/box_builder.h>
#include <base/logger.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "actuator_ventilation.h"
#include "average_cover.h"
#include "average_screen.h"
#include "budget.h"
#include "budget_layer.h"
#include "budget_volume.h"
#include "floor.h"
#include "growth_lights.h"
#include "heat_pipes.h"
#include "layer_adjusted.h"
#include "plant.h"
#include "sky.h"

#include <base/dialog.h>

using namespace base;
using namespace phys_math;
using namespace TestNum;

namespace {
    double integrate(double x, double y0, double k, double v, double z) {
        return eqZero(v) ?
               y0 + k*x :
               exp(-v*x)*( y0 + (k/v + z)*(exp(v*x)-1.) );
    }
}

namespace vg {

PUBLISH(Budget)

Budget::Budget(QString name, base::Box *parent)
    : Box(name, parent)
{
    help("resolves energy and water budgets across layers and volumes");
    Input(radPrecision).equals(0.1).unit("W/m2|mumol/m2/s").help("Precision of numerical solution to radiation budget");
    Input(tempPrecision).equals(0.5).unit("K").help("Max. allowed temperature change in a sub-step among layers");
    Input(thresholdPrecision).equals(0.1).unit("K").help("Precision of temperature thresholds for climate control");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Input(averageHeight).imports("gh/geometry[averageHeight]");
    Input(coverPerGroundArea).imports("gh/geometry[coverPerGroundArea]");
    Input(outdoorsTemperature).imports("outdoors[temperature]");
    Input(outdoorsRh).imports("outdoors[rh]");
    Input(outdoorsCo2).imports("outdoors[co2]");
    Input(transpirationRate).imports("gh/plant[transpiration]");
    Input(Pn).imports("gh/plant[Pn]");
    Input(co2Injection).imports("gh/actuators/co2[value]");
    Input(heatPipeFlux).imports("gh/actuators/heatPipes[heatFlux]");
    Input(ventilationThreshold).imports("gh/controllers/ventilation/temperatureThreshold[value]");
    Input(ventilationCostThreshold).imports("gh/controllers/ventilation/maxHeatingCost[value]");
    Input(heatingThreshold).imports("gh/controllers/heating[value]");
    Input(heatPipesOn).imports("gh/actuators/heatPipes/*[isHeating]");
    Input(ventilationOn).imports("gh/actuators/ventilation[isVentilating]");
    Input(deltaVentControl).equals(0.3).unit("/h/min").help("Control increment of ventilation flux");
    Input(deltaVentControlRelative).equals(0.2).unit("/min").help("Relative control of ventilation flux");
    Input(deltaHeatingControl).equals(4.0).unit("K/min").help("Control increment in heating temperature");
    Input(babyTimeStep).equals(1.).unit("s").help("Length of first time step after climate control action");

    Input(step).imports("sim[step]");
    Input(dateTime).imports("calendar[dateTime]");

    Output(subSteps).unit("int").help("Number of sub-steps taken to resolve the whole budget");
    Output(radIterations).unit("int").help("Number of iterations taken to resolve radiation budget");
    Output(maxDeltaT).unit("K").help("Max. temperature change in a sub-step");
    Output(controlCode).unit("int").help("Code for the control option needed");
    Output(actionCode).unit("int").help("Code for the control action taken");
    Output(transpiration).unit("kg/m2").help("Plant transpiration");
    Output(condensation).unit("kg/m2").help("Condensation on cover");
    Output(ventedWater).unit("kg/m2").help("Water loss by ventilation");
    Output(latentHeatBalance).unit("W/m2").help("Latent heat of water balance");
    Output(ventilationHeatLoss).unit("W/m2").help("Sensible heat lost by ventilation");

    Output(indoorsSensibleHeatFlux).unit("W/m2").help("Rate of change in indoors air sensible heat");
    Output(indoorsLatentHeatFlux).unit("W/m2").help("Rate of change in indoors air latent heat");
}

void Budget::amend() {
    auto childBoxes = findMany<Box*>("./*");
    if (!childBoxes.isEmpty())
        ThrowException("Budget must be declared empty in the boxscript, i.e. as \"Budget budget{ }\"").
                value("\n" + Node::fullNames(childBoxes).join("\n")).context(this);
    addVolumes();
    addLayers();
    addState();
    addParameters();
}

void Budget::addVolumes() {
    // Add volumes as children
    BoxBuilder builder(this);
    builder.
        box("BudgetVolume").name("outdoorsVol").
            port("initTemperature").imports("/outdoors[temperature]").
            port("temperature").imports("/outdoors[temperature]").
            port("initRh").imports("/outdoors[rh]").
            port("rh").imports("/outdoors[rh]").
        endbox().
        box("BudgetVolume").name("indoors").
            port("initTemperature").equals(20.).
            port("initRh").equals(70).
        endbox().
        box("BudgetVolume").name("soilVol").
            port("initTemperature").imports("soilTemperature[value]").
            port("temperature").imports("soilTemperature[value]").
        endbox();

    // Find volumes
    volumes << (outdoorsVol = findOne<BudgetVolume*>("./outdoorsVol"));
    volumes << (indoorsVol  = findOne<BudgetVolume*>("./indoors"));
    volumes << (soilVol     = findOne<BudgetVolume*>("./soilVol"));
}

void Budget::addLayers() {
    // Find layers
    sky          = findOne<Sky*>("/sky");
    cover        = findOne<AverageCover*>("shelter/layers/cover");
    screens      = findMany<AverageScreen*>("shelter/layers/screens/*");
    growthLights = findMaybeOne<GrowthLights*>("actuators/growthLights");
    heatPipes    = findMaybeOne<HeatPipes*>("actuators/heatPipes");
    plant        = findMaybeOne<Plant*>("gh/plant");
    floor        = findOne<Floor*>("gh/floor");

    // Build layers as children
    BoxBuilder builder(this);
    builder.
        box("BudgetLayerSky").name("sky").
            port("temperature").imports("/sky[temperature]").
            port("swEmissionBottom").imports("outdoors[radiation]").
            port("parEmissionBottom").imports("outdoors[par]").
        endbox().
        box("BudgetLayerCover").name("cover").
        endbox();
    QStringList screenNames;
    for (AverageScreen *screen : screens) {
        builder.
            box("BudgetLayerScreen").name(screen->objectName()).
            endbox();
        screenNames << screen->objectName();
    }
    if (growthLights) {
        builder.
            box("BudgetLayer").name("growthLights").
                port("parEmissionBottom").imports("actuators/growthLights[parEmissionBottom]").
                port("swEmissionBottom") .imports("actuators/growthLights[swEmissionBottom]").
                port("lwEmissionTop")    .imports("actuators/growthLights[lwEmissionTop]").
                port("lwEmissionBottom") .imports("actuators/growthLights[lwEmissionBottom]").
                port("convectionTop")    .imports("actuators/growthLights[convectionTop]").
                port("convectionBottom") .imports("actuators/growthLights[convectionBottom]").
            endbox();
    }
    if (plant) {
        builder.
            box("BudgetLayer").name("plant").
                port("initTemperature").imports("gh/plant[temperature]").
                port("temperature").imports("gh/plant[temperature]").
            endbox();
    }
    if (heatPipes) {
        builder.
            box("BudgetLayer").name("heatPipes").
                port("lwEmissionTop")   .imports("actuators/heatPipes[lwEmissionTop]").
                port("lwEmissionBottom").imports("actuators/heatPipes[lwEmissionBottom]").
                port("convectionTop")   .imports("actuators/heatPipes[convectionTop]").
                port("convectionBottom").imports("actuators/heatPipes[convectionBottom]").
                port("temperature")     .imports("actuators/heatPipes[inflowTemperatureAvg]").
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
    budgetLayerCover = findOne<BudgetLayer*>("./cover");
    budgetLayerCover->attach(cover, outdoorsVol, indoorsVol);
    layers << budgetLayerCover;

    // Attach screens
    int i = 0;
    for (const QString &screenName : screenNames) {
        BudgetLayer *budgetLayerScreen = findOne<BudgetLayer*>("./"+screenName);
        budgetLayerScreen->attach(screens[i++], indoorsVol, indoorsVol);
        layers << budgetLayerScreen;
    }

    // Attach growth lights
    if (growthLights) {
        BudgetLayer *budgetLayerGrowthLights = findOne<BudgetLayer*>("./growthLights");
        budgetLayerGrowthLights->attach(growthLights, indoorsVol, indoorsVol);
        layers << budgetLayerGrowthLights;
    }

    // Attach plant
    if (plant) {
        budgetLayerPlant = findOne<BudgetLayer*>("./plant");
        budgetLayerPlant->attach(plant, indoorsVol, indoorsVol);
        layers << budgetLayerPlant;
    }

    // Attach heat pipes
    if (heatPipes) {
        budgetLayerHeatPipes = findOne<BudgetLayer*>("./heatPipes");
        budgetLayerHeatPipes->attach(heatPipes, indoorsVol, indoorsVol);
        layers << budgetLayerHeatPipes;
    }
    else {
        budgetLayerHeatPipes = nullptr;
    }

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
        swParam.a   << &layer->attachedLayer->swAbsorptivityTop;
        swParam.a_  << &layer->attachedLayer->swAbsorptivityBottom;
        swParam.r   << &layer->attachedLayer->swReflectivityTop;
        swParam.r_  << &layer->attachedLayer->swReflectivityBottom;
        swParam.t   << &layer->attachedLayer->swTransmissivityTop;
        swParam.t_  << &layer->attachedLayer->swTransmissivityBottom;

        lwParam.a   << &layer->attachedLayer->lwAbsorptivityTop;
        lwParam.a_  << &layer->attachedLayer->lwAbsorptivityBottom;
        lwParam.r   << &layer->attachedLayer->lwReflectivityTop;
        lwParam.r_  << &layer->attachedLayer->lwReflectivityBottom;
        lwParam.t   << &layer->attachedLayer->lwTransmissivityTop;
        lwParam.t_  << &layer->attachedLayer->lwTransmissivityBottom;
    }
}

void Budget::initialize() {
    actuatorVentilation = findOne<ActuatorVentilation*>("actuators/ventilation");
    ventilationRate = actuatorVentilation->port("value")->valuePtr<double>();
    indoorsHeatCapacity = RhoAir*CpAir*averageHeight;

//    logger.open("C:/MyDocuments/QDev/UniSim3/output/unisim_log.txt");
}

void Budget::reset() {
    control = Control::CarryOn;
    action = Action::CarryOn;
    _subTimeStep = timeStep;
    _maxDeltaT = tempPrecision;
    // Correct baby step to be max 1% of the time step
    babyTimeStep = std::min(babyTimeStep, timeStep/100.);
}

void Budget::update() {
    const double
            indoorsTemp0 = indoorsVol->temperature,
            indoorsRh0   = indoorsVol->rh;
    updateLayersAndVolumes();
    updateCo2();
    exertControl();

    const double
            indoorsTemp1 = indoorsVol->temperature,
            indoorsRh1   = indoorsVol->rh;
    indoorsSensibleHeatFlux = (indoorsTemp1 - indoorsTemp0)*RhoAir*CpAir*averageHeight/timeStep;
    indoorsLatentHeatFlux = (ahFromRh(indoorsTemp0, indoorsRh0) - ahFromRh(indoorsTemp1, indoorsRh1))*LHe*averageHeight/timeStep;
}

void Budget::updateLayersAndVolumes() {
    const int maxSubSteps = 1000;
    saveForRollBack();
    maxDeltaT = 0.;
    subSteps = 0;
    double timePassed = 0.;
    babyStep();
    while (lt(timePassed, timeStep) && subSteps < maxSubSteps) {
        _subTimeStep = std::min(_subTimeStep*tempPrecision/_maxDeltaT, timeStep - timePassed);
        updateSubStep(_subTimeStep, (timePassed == 0.) ? UpdateOption::IncludeSwPar : UpdateOption::ExcludeSwPar);
        plant->updateByRadiation(budgetLayerPlant->netRadiation,
                                 budgetLayerPlant->parAbsorbedTop +
                                 budgetLayerPlant->parAbsorbedBottom);
        updateWaterBalance(_subTimeStep);
        applyDeltaT();
        applyLatentHeat();
        timePassed += _subTimeStep;
        ++subSteps;
    }
    if (subSteps == maxSubSteps)
        dialog().error("On" + convert<QString>(dateTime) + ": Energy budget did not converge");
//    logger.close();
}

void Budget::updateSubStep(double subTimeStep, UpdateOption option) {
    // Reset to find maximum temperature change during sub-step
    _maxDeltaT = 0.;
    // Update radiation budget
    updateLwEmission();
    if (option == UpdateOption::IncludeSwPar) {
        swState.init();
        parState.init();
    }
    lwState.init();

    if (option == UpdateOption::IncludeSwPar) {
        distributeRadiation(swState,  swParam);
        distributeRadiation(parState, swParam);
    }
    distributeRadiation(lwState,  lwParam);

    updateNetRadiation();
    // Update convection/conduction budget
    updateConvection();
    // Update prospective change in temperature
    updateDeltaT(subTimeStep);
    // Update max temperature within simulation time step
    if (_maxDeltaT > maxDeltaT)
        maxDeltaT = _maxDeltaT;
}

void Budget::updateLwEmission() {
    for (BudgetLayer *layer : layers)
        layer->updateLwEmission();
}

void Budget::updateNetRadiation() {
    for (BudgetLayer *layer : layers)
        layer->updateNetRadiation();
}

void Budget::updateConvection() {
    for (BudgetVolume *volume : volumes)
        volume->heatInflux = 0.;
    for (BudgetLayer *layer : layers)
        layer->updateConvection();
}

namespace {
struct WaterIntegration {
    double
        indoorsAh,
        deltaAH,
        transpiration,
        condensation,
        ventilation;
};

WaterIntegration waterIntegration(
        double x,  // time step
        double y0, // indoors ah
        double T,  // transpiration
        double cn, // condensation rate
        double C,  // sah of cover
        double v,  // ventilation rate
        double V)  // outdoors ah
{
    if (y0 < C)
        cn = 0.;
    const double
        indoorsAh0    = y0,
        indoorsAh1    = (cn + v > 0.) ? (-exp(x*(-(cn + v)))*(cn*C - y0*(cn + v) + T + v*V) + cn*C + T + v*V)/(cn + v) : 0.,
        avgAh         = (indoorsAh0 + indoorsAh1)/2,
        transpiration = T*x,
        condensation  = std::max(cn*(avgAh - C)*x, 0.),
        ventilation   = v*(avgAh-V)*x;

    return WaterIntegration{
        indoorsAh1,
        indoorsAh1 - indoorsAh0,
        transpiration,
        condensation,
        ventilation
    };
}

}

void Budget::updateWaterBalance(double timeStep) {
    const double
       indoorsAh  = ahFromRh(indoorsVol->temperature,  indoorsVol->rh),
       outdoorsAh = ahFromRh(outdoorsTemperature, outdoorsRh),
       coverSah   = sah(budgetLayerCover->temperature),
       v          = (*ventilationRate)/3600.;

    WaterIntegration w = waterIntegration(
        timeStep,
        indoorsAh,
        transpirationRate/averageHeight,
        2e-3*coverPerGroundArea/averageHeight,
        coverSah,
        v,
        outdoorsAh
    );

    transpiration     =  w.transpiration*averageHeight;
    condensation      = -w.condensation*averageHeight;
    ventedWater       = -w.ventilation*averageHeight;
    latentHeatBalance = -LHe*condensation;
    indoorsVol->rh    = rhFromAh(indoorsVol->temperature, w.indoorsAh);
}

void Budget::applyLatentHeat() {
//    budgetLayerCover->temperature += latentHeatBalance/(*budgetLayerCover->heatCapacity);
    const double Cair = RhoAir*CpAir;
    indoorsVol->temperature += latentHeatBalance/averageHeight/Cair;
}


void Budget::updateCo2() {
    double
        indoorsCo2  = 1.829e-3*indoorsVol->co2,
        outdoorsCo2 = 1.829e-3*Budget::outdoorsCo2,
        injection   = co2Injection/3600.,
        fixation    = Pn*44.01e-6/averageHeight,
        c           = injection - fixation,
        v           = (*ventilationRate)/3600.;

    if (eqZero(v)) {
        indoorsCo2 += c*timeStep;
    }
    else {
        indoorsCo2 = integrate(timeStep, indoorsCo2, c, v, outdoorsCo2);
    }
    indoorsVol->co2 = indoorsCo2/1.829e-3;
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
    const int maxIterations = 99;
    radIterations = 0;
    double residual;
//    dialog().information(dump(p, Dump::WithHeader));
//    dialog().information(dump(s, Dump::WithHeader));
    do {
        ++radIterations;
//        dialog().information(QString::number(radIterations) + " " + QString::number(residual));
        distributeRadDown(s, p);
        distributeRadUp(s, p);
//        dialog().information(dump(s));
        // Don't include upwards radiation (F_) from sky layer in residual
        residual = *s.F.at(0);
        for (int i=1; i<numLayers; ++i)
            residual += *s.F.at(i) + *s.F_.at(i);
    } while (residual > radPrecision && radIterations < maxIterations);
    if (radIterations == maxIterations)
       ThrowException("Radiation budget did not converge").value(residual).
                 hint("Check your parameters:\n" + dump(p, Dump::WithHeader)).context(this);
}

void Budget::updateDeltaT(double timeStep) {
    for (BudgetLayer *layer : layers) {
        double deltaT = layer->updateDeltaT(timeStep);
        if (fabs(deltaT) > fabs(_maxDeltaT))
            _maxDeltaT = fabs(deltaT);
    }
    double propVentilation   = 1. - exp(-(*ventilationRate)/3600.*timeStep),
           ventilationDeltaT = (outdoorsTemperature - indoorsVol->temperature)*propVentilation;
    indoorsDeltaT = indoorsVol->heatInflux*timeStep/indoorsHeatCapacity + ventilationDeltaT;
    ventilationHeatLoss = ventilationDeltaT*averageHeight*RhoAir*CpAir/timeStep;
    if (fabs(indoorsDeltaT) > fabs(_maxDeltaT))
        _maxDeltaT = fabs(indoorsDeltaT);
}

void Budget::applyDeltaT() {
    for (BudgetLayer *layer : layers)
        layer->temperature += layer->totalDeltaT;
    indoorsVol->temperature += indoorsDeltaT;
}

void Budget::saveForRollBack() {
    for (BudgetLayer *layer : layers)
         layer->rollBackTemperature = layer->temperature;
    indoorsVol->rollBackTemperature = indoorsVol->temperature;
    indoorsVol->rollBackRh          = indoorsVol->rh;
}

void Budget::rollBack() {
    for (BudgetLayer *layer : layers)
        layer->temperature = layer->rollBackTemperature;
    indoorsVol->temperature = indoorsVol->rollBackTemperature;
    indoorsVol->rh = indoorsVol->rollBackRh;
}

namespace {

bool any(QVector<bool> flags) {
    for (bool flag : flags)
        if (flag) return true;
    return false;
}

}

void Budget::diagnoseControl() {
    const double &T(indoorsVol->temperature);
    if (fabs(T - ventilationThreshold) < thresholdPrecision)
        control = Control::OnSetpointVentilation;
    else if (fabs(T - heatingThreshold) < thresholdPrecision)
        control = Control::OnSetpointHeating;
    else if (T > ventilationThreshold)
        control = Control::GreenhouseTooHot;
    else if (T < heatingThreshold)
        control = Control::GreenhouseTooCold;
    else if (any(heatPipesOn))
        control = Control::NeedlessHeating;
    else if (ventilationOn)
        control = Control::NeedlessCooling;
    else control = Control::CarryOn;
    greenhouseTooHumid   = (ventilationCostThreshold > 0.);
    tooCostlyVentilation = (ventilationOn && heatPipeFlux > ventilationCostThreshold);
    controlCode = static_cast<int>(control);
}

void Budget::exertControl() {
    diagnoseControl();
    switch (control) {
    case Control::GreenhouseTooHot:
        if (any(heatPipesOn))
            decreaseHeating();
        else
            increaseVentilation();
        break;
    case Control::GreenhouseTooCold:
        if (tooCostlyVentilation)
            decreaseVentilation();
        else
            increaseHeating();
        break;
    case Control::NeedlessHeating:
        decreaseHeating();
        break;
    case Control::NeedlessCooling:
        if (!greenhouseTooHumid)
            decreaseVentilation();
        break;
    case Control::OnSetpointVentilation:
    case Control::OnSetpointHeating:
    case Control::CarryOn:
        if (tooCostlyVentilation)
            decreaseVentilation();
        else if (greenhouseTooHumid)
            increaseVentilation();
        else
            action = Action::CarryOn;
        break;
    }
    actionCode = static_cast<int>(action);
}

void Budget::increaseVentilation() {
    rollBack();
    actuatorVentilation->increase(deltaVentControl*timeStep/60.);
    babyStep();
    updateLayersAndVolumes();
    action = Action::IncreaseVentilation;
}

void Budget::decreaseVentilation() {
    rollBack();
    actuatorVentilation->decrease(-deltaVentControl*timeStep/60., deltaVentControlRelative*timeStep/60.);
    babyStep();
    updateLayersAndVolumes();
    action = Action::DecreaseVentilation;
}

void Budget::increaseHeating() {
    if (heatPipes) {
        rollBack();
        heatPipes->increase(deltaHeatingControl*timeStep/60.);
        budgetLayerHeatPipes->evaluatePorts();
        extraVentilation();
        babyStep();
        updateLayersAndVolumes();
    }
    action = Action::IncreaseHeating;
}

void Budget::decreaseHeating() {
    if (heatPipes) {
        rollBack();
        heatPipes->increase(-deltaHeatingControl*timeStep/60.);
        budgetLayerHeatPipes->evaluatePorts();
        extraVentilation();
        babyStep();
        updateLayersAndVolumes();
    }
    action = Action::DecreaseHeating;
}

void Budget::extraVentilation() {
    if (greenhouseTooHumid) {
        babyStep();
        actuatorVentilation->increase(deltaVentControl*timeStep/60.);
    }
}

void Budget::babyStep() {
    // Force a tentative, short _subTimeStep to assess the rate of temperature change
    _maxDeltaT = _subTimeStep*tempPrecision/babyTimeStep;
}

QString Budget::dump(const State &s, Dump header) {
    QString string;
    QTextStream result(&string);
    if (header == Dump::WithHeader)
        result << "i\tlayer\tE\tE_\tF\tF_\tA\tA_\tT\n";
    int n = s.E.size();
    for (int i = 0; i < n; ++i) {
        result
            << i << "\t"
            << layers.at(i)->name() << "\t"
            << *s.E.at(i) << "\t"
            << *s.E_.at(i) << "\t"
            << *s.F.at(i) << "\t"
            << *s.F_.at(i) << "\t"
            << *s.A.at(i) << "\t"
            << *s.A_.at(i) << "\t"
            << layers.at(i)->temperature << "\n";
    }
    return string;
}

QString Budget::dump(const Parameters &p, Dump header) {
    QString string;
    QTextStream result(&string);
    if (header == Dump::WithHeader)
        result << "i\tlayer\ta\ta_\tr\tr_\tt\tt_\n";
    int n = p.a.size();
    for (int i = 0; i < n; ++i) {
        result
            << i << "\t"
            << layers.at(i)->name() << "\t"
            << *p.a.at(i) << "\t"
            << *p.a_.at(i) << "\t"
            << *p.r.at(i) << "\t"
            << *p.r_.at(i) << "\t"
            << *p.t.at(i) << "\t"
            << *p.t_.at(i) << "\n";
    }
    return string;
}


}
