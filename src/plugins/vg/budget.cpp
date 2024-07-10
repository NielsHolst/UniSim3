/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <QTextStream>
#include <base/box_builder.h>
#include <base/environment.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "actuator_ventilation.h"
#include "average_cover.h"
#include "average_screen.h"
#include "budget.h"
#include "budget_layer.h"
#include "budget_layer_cover.h"
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
    Input(radPrecision).equals(0.1).unit("W/m2 | &micro;mol/m2/s").help("Precision of numerical solution to radiation budget");
    Input(tempPrecision).equals(2.0).unit("K").help("Max. allowed temperature change in a sub-step among layers");
    Input(writeHighRes).equals(false).help("Write output at finest time resolution (indicated by `subDateTime`)");
    Input(writeLog).equals(false).help("Write log output");
    Input(controlClimate).equals(true).help("Should climate be controlled according to setpoints?");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Input(averageHeight).imports("gh/geometry[averageHeight]");
    Input(groundArea).imports("gh/geometry[groundArea]");
    Input(coverPerGroundArea).imports("gh/geometry[coverPerGroundArea]");
    Input(outdoorsTemperature).imports("outdoors[temperature]");
    Input(outdoorsRh).imports("outdoors[rh]");
    Input(outdoorsCo2).imports("outdoors[co2]");
    Input(transpirationRate).imports("gh/plant[transpiration]");
    Input(humidificationRate).imports("actuators/humidifiers[vapourFlux]");
    Input(Pn).imports("gh/plant[Pn]");
    Input(co2Injection).imports("gh/actuators/co2[value]");
    Input(heatPipeFlux).imports("gh/actuators/heatPipes[heatFlux]");
    Input(heatPumpCooling).imports("actuators/heatPumps[cooling]");
    Input(heatPumpCondensationRate).imports("actuators/heatPumps[condensation]");
    Input(padAndFanCooling).imports("actuators/padAndFans[cooling]");
    Input(padAndFanVapourFlux).imports("actuators/padAndFans[vapourFlux]");
    Input(heatPipesOn).imports("gh/actuators/heatPipes/*[isHeating]");
    Input(isVentilating).imports("gh/actuators/ventilation[isVentilating]");
    Input(isHeating).imports("gh/actuators/heatPipes[isHeating]");
    Input(babyTimeStep).equals(1.).unit("s").help("Length of first time step after climate control action");
    Input(step).imports("/.[step]");
    Input(dateTime).imports("calendar[dateTime]");

    Output(subDateTime).help("Date time within integration time step");
    Output(subTimeStep).unit("s").help("Length of integration sub-time step");
    Output(subSteps).help("Number of sub-steps taken to resolve the whole budget");
    Output(radIterations).help("Number of iterations taken to resolve radiation budget");
    Output(maxDeltaT).unit("K").help("Max. temperature change in a sub-step");
    Output(transpiration).unit("kg/m2").help("Plant transpiration");
    Output(condensationCover).unit("kg/m2").help("Condensation on cover");
    Output(condensationHeatPump).unit("kg/m2").help("Water removed by heat pumps");
    Output(ventedWater).unit("kg/m2").help("Water loss by ventilation");
    Output(ventilationHeatLoss).unit("W/m2").help("Sensible heat lost by ventilation");
    Output(indoorsSensibleHeatFlux).unit("W/m2").help("Rate of change in indoors air sensible heat");
    Output(indoorsLatentHeatFlux).unit("W/m2").help("Rate of change in indoors air latent heat");
    Output(coverLatentHeatFlux).unit("W/m2").help("Rate of condensation heat influx to cover");
    Output(sunParAbsorbedInCover).unit("&micro;mol/m2/s").help("Ignoring transmission and reflection of sunlight");
    Output(sunParAbsorbedInScreens).unit("&micro;mol/m2/s").help("Ignoring transmission and reflection of sunlight");
    Output(sunParHittingPlant).unit("&micro;mol/m2/s").help("Sunlight PAR hitting plant canopy");
    Output(growthLightParHittingPlant).unit("&micro;mol/m2/s").help("Growth light PAR hitting plant canopy");
    Output(totalPar).unit("&micro;mol/m2/s").help("Total PAR hitting plant canopy");
    Output(waterBudgetCorr);
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
    heatingActuator = findOne<HeatPipes*>("actuators/heatPipes");
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
            box("BudgetLayerGrowthLights").name("growthLights").
            endbox();
    }
    if (plant) {
        builder.
            box("BudgetLayer").name("plant").
                port("initTemperature").imports("gh/plant[temperature]").
                port("temperature").imports("gh/plant[temperature]").
            endbox();
    }
    builder.
        box("BudgetLayerHeatPipes").name("heatPipes").
        endbox();
    builder.
        box("BudgetLayer").name("floor").
    endbox();


    // Attach sky
    budgetLayerSky = findOne<BudgetLayer*>("./sky");
    budgetLayerSky->attach(sky, nullptr, nullptr);
    layers << budgetLayerSky;

    // Attach cover
    budgetLayerCover = findOne<BudgetLayerCover*>("./cover");
    budgetLayerCover->attach(cover, outdoorsVol, indoorsVol);
    layers << budgetLayerCover;

    // Attach screens
    int i = 0;
    for (const QString &screenName : screenNames) {
        BudgetLayer *budgetLayerScreen = findOne<BudgetLayer*>("./"+screenName);
        budgetLayerScreen->attach(screens[i], indoorsVol, indoorsVol);
        layers << budgetLayerScreen;
        ++i;
    }

    // Attach growth lights
    if (growthLights) {
        budgetLayerGrowthLights = findOne<BudgetLayer*>("./growthLights");
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
    budgetLayerHeatPipes = findOne<BudgetLayer*>("./heatPipes");
    budgetLayerHeatPipes->attach(heatingActuator, indoorsVol, indoorsVol);
    layers << budgetLayerHeatPipes;

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
        // Note: Absorption occurs in opposite direction of emission and flow
        swState.E  << &layer->swEmissionBottom;
        swState.E_ << &layer->swEmissionTop;
        swState.F  << &layer->swFlowBottom;
        swState.F_ << &layer->swFlowTop;
        swState.A_ << &layer->swAbsorbedBottom;
        swState.A  << &layer->swAbsorbedTop;

        lwState.E  << &layer->lwEmissionBottom;
        lwState.E_ << &layer->lwEmissionTop;
        lwState.F  << &layer->lwFlowBottom;
        lwState.F_ << &layer->lwFlowTop;
        lwState.A_ << &layer->lwAbsorbedBottom;
        lwState.A  << &layer->lwAbsorbedTop;

        parState.E  << &layer->parEmissionBottom;
        parState.E_ << &layer->parEmissionTop;
        parState.F  << &layer->parFlowBottom;
        parState.F_ << &layer->parFlowTop;
        parState.A_ << &layer->parAbsorbedBottom;
        parState.A  << &layer->parAbsorbedTop;
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
    indoorsHeatCapacity = RhoAir*CpAir*averageHeight;
    // Hand-held setpoints, controllers and actuators
    handheldBoxes = findMany<Box*>(
        "setpoints/heating|"
        "setpoints/heatPumps|"
        "setpoints/ventilation|"
        "setPoints/padAndFans|"
        "setPoints/humidification|"

        "controllers/desiredMinTemperature|"
        "controllers/heatPipes|"
        "controllers/heatPumps|"
        "controllers/ventilation|"
        "controllers/padAndFans|"
        "controllers/humidification|"

        "actuators/heatPipes|"
        "actuators/heatPumps|"
        "actuators/ventilation|"
        "actuators/padAndFans|"
        "actuators/humidifiers"
    );
    ventilationRate = findOne<ActuatorVentilation*>("actuators/ventilation")->port("value")->valuePtr<double>();
    // Find writer for detailed output
    outputWriter = findMaybeOne<Box*>("outputWriter");
    if (!outputWriter)
        writeHighRes = false;
}

void Budget::reset() {
    checkParameters();
    subTimeStep = timeStep;
    subDateTime = dateTime;
    _maxDeltaT = tempPrecision;
    // Correct baby step to be max 1% of the time step
    babyTimeStep = std::min(babyTimeStep, timeStep/100.);
    // Log output
    if (writeLog)
        logger.open(environment().outputFileNamePath("vg_log.txt"));
}

void Budget::update() {
    checkParameters();
    const double
            indoorsTemp0 = indoorsVol->temperature,
            indoorsRh0   = indoorsVol->rh;
    updateInSubSteps();
    updateCo2();

    const double
            indoorsTemp1 = indoorsVol->temperature,
            indoorsRh1   = indoorsVol->rh;
    indoorsSensibleHeatFlux = (indoorsTemp1 - indoorsTemp0)*RhoAir*CpAir*averageHeight/timeStep;
    indoorsLatentHeatFlux = (ahFromRh(indoorsTemp0, indoorsRh0) - ahFromRh(indoorsTemp1, indoorsRh1))*LHe*averageHeight/timeStep;

    // Approximate outputs
    growthLightParHittingPlant = budgetLayerGrowthLights->parEmissionBottom;
    totalPar = budgetLayerPlant->parAbsorbedTop    / budgetLayerPlant->attachedLayer->swAbsorptivityTop +
               budgetLayerPlant->parAbsorbedBottom / budgetLayerPlant->attachedLayer->swAbsorptivityBottom;
    sunParHittingPlant = std::max(totalPar - growthLightParHittingPlant, 0.);

    if (writeLog && !writeHighRes)
        writeToLog();
}

void Budget::cleanup() {
    if (writeLog)
        logger.close();
}

void Budget::updateInSubSteps() {
    const int maxSubSteps = 1000;
    maxDeltaT = 0.;
    subSteps = 0;
    double timePassed = 0.;
    subDateTime = dateTime;
    babyStep();
    transpiration =
    condensationCover  =
    ventedWater   =
    condensationHeatPump = 0.;
    while (lt(timePassed, timeStep) && subSteps < maxSubSteps) {
        subTimeStep = std::min(subTimeStep*tempPrecision/_maxDeltaT, timeStep - timePassed);

        // Update deltaT for all volumes and layers, except plant
        updateSubStep(subTimeStep, (timePassed == 0.) ? UpdateOption::IncludeSwPar : UpdateOption::ExcludeSwPar);

        // Update plant
        plant->updateByRadiation(budgetLayerPlant->netRadiation,
                                 budgetLayerPlant->parAbsorbedTop +
                                 budgetLayerPlant->parAbsorbedBottom);
        port("transpirationRate")->evaluate();

        // Update water balance
        updateWaterBalance(subTimeStep);

        // Update all temperatures
        applyDeltaT();

        // Effectuate climate control
        if (controlClimate) {
            for (Box *box : handheldBoxes)
                box->updateFamily();
        }

        // Keep track of time passed
        timePassed += subTimeStep;
        subDateTime = dateTime.addMSecs(static_cast<int>(1000.*timePassed));

        // Force written output according to option
        if (writeHighRes && lt(timePassed, timeStep)) {
            outputWriter->cleanupFamily();
            if (writeLog)
                writeToLog();
        }

        // Count sub-steps with major simulation time step
        ++subSteps;
    }
    if (subSteps == maxSubSteps)
        dialog().error("On" + convert<QString>(dateTime) + ": Energy budget did not converge");
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
    distributeRadiation(lwState, lwParam);

    updateConvection();
    updateDeltaT(subTimeStep);
    // Update max temperature within simulation time step
    if (_maxDeltaT > maxDeltaT)
        maxDeltaT = _maxDeltaT;
}

void Budget::updateLwEmission() {
    for (BudgetLayer *layer : layers)
        layer->updateLwEmission();
}

void Budget::updateConvection() {
    for (BudgetVolume *volume : volumes)
        volume->heatInflux = 0.;
    for (BudgetLayer *layer : layers)
        layer->updateConvection();
}

void Budget::updateDeltaT(double timeStep) {
    for (BudgetLayer *layer : layers) {
        double deltaT = layer->updateDeltaT(timeStep);
        if (fabs(deltaT) > _maxDeltaT)
            _maxDeltaT = fabs(deltaT);
    }
    double propVentilation   = 1. - exp(-(*ventilationRate)/3600.*timeStep),
           ventilationDeltaT = (outdoorsTemperature - indoorsVol->temperature)*propVentilation;
    indoorsDeltaT = (indoorsVol->heatInflux - (heatPumpCooling + padAndFanCooling))*timeStep/indoorsHeatCapacity + ventilationDeltaT;
    ventilationHeatLoss = ventilationDeltaT*averageHeight*RhoAir*CpAir/timeStep;
    if (fabs(indoorsDeltaT) > fabs(_maxDeltaT))
        _maxDeltaT = fabs(indoorsDeltaT);
}

void Budget::applyDeltaT() {
    for (BudgetLayer *layer : layers)
        layer->temperature += layer->totalDeltaT;
    indoorsVol->temperature += indoorsDeltaT;
}

namespace {

struct WaterIntegration {
    double
        deltaAh,
        condensation,
        ventilation;
};

WaterIntegration waterIntegration(
        double x,  // time step
        double y0, // indoors ah
        double ET, // evapotranspiration
        double c,  // cover condensation rate
        double C,  // sah of cover
        double v,  // ventilation rate
        double V)  // outdoors ah
{
    if (y0 < C)
        c = 0.;
    const double
        indoorsAh0    = y0,
        tau           = exp(-x*(c+v)),
        indoorsAh1    = (c + v > 0.) ? (ET + c*C + v*V - tau*(c*C - y0*(c + v) + ET + v*V))/(c + v) : 0.,
//        indoorsAh2    = (c + v > 0.) ? ((1. - tau)*ET + c*(C - tau*(C - y0)) + v*(V - tau*(V - y0)))/(c + v) : 0.,
        avgAh         = (indoorsAh0 + indoorsAh1)/2.,
        condensation  = std::max(c*(avgAh - C)*x, 0.),
        ventilation   = v*(avgAh-V)*x;
//    if (TestNum::ne(indoorsAh1, indoorsAh2, 0.01))
//        ThrowException("Water integration error").value(indoorsAh1).value2(indoorsAh2);

    return WaterIntegration{
        indoorsAh1 - indoorsAh0,
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
       c          = 2e-3*coverPerGroundArea/averageHeight,
       v          = (*ventilationRate)/3600.;

    WaterIntegration w = waterIntegration(
        timeStep,
        indoorsAh,
        (transpirationRate + humidificationRate + padAndFanVapourFlux - heatPumpCondensationRate)/averageHeight,
        c,
        coverSah,
        v,
        outdoorsAh
    );
    // Deltas
    double
        // kg/m3 * m   = kg/m2
        d_condensationCover    = w.condensation*averageHeight,
        d_ventedWater          = w.ventilation*averageHeight,
        // kg/m2/s * s = kg/m2
        d_transpiration        = transpirationRate*timeStep,
        d_humidification       = humidificationRate*timeStep,
        d_condensationHeatPump = heatPumpCondensationRate*timeStep;
    const double
        d_sum =
           - d_condensationCover
           - d_ventedWater
           + d_transpiration
           + d_humidification
           - d_condensationHeatPump,
        correction = w.deltaAh/d_sum;

    d_condensationCover    *= correction;
    d_ventedWater          *= correction;
    d_transpiration        *= correction;
    d_humidification       *= correction;
    d_condensationHeatPump *= correction;
    waterBudgetCorr = correction;

    double correctedSum =
            - d_condensationCover
            - d_ventedWater
            + d_transpiration
            + d_humidification
            - d_condensationHeatPump;
    if (TestNum::ne(w.deltaAh, correctedSum, 0.01))
        ThrowException("Mismath").value(w.deltaAh).value2(correctedSum);

    // Outputs
    condensationCover    -= d_condensationCover;
    ventedWater          -= d_ventedWater;
    transpiration        += d_transpiration;
    condensationHeatPump -= d_condensationHeatPump;

    // Indoors state update
    indoorsVol->rh     = rhFromAh(indoorsVol->temperature, indoorsAh + w.deltaAh);

    // Outside latent heat
    const double
       condRate   = 2e-3*coverPerGroundArea,
       outsideCondensation  = std::max(condRate*(outdoorsAh - coverSah)*timeStep, 0.),
       coverCondensation = d_condensationCover + outsideCondensation;

    // Update cover temperature
    coverLatentHeatFlux = LHe*coverCondensation;
    double deltaT = budgetLayerCover->updateDeltaTByCondensation(coverCondensation);
    if (fabs(deltaT) > _maxDeltaT)
        _maxDeltaT = fabs(deltaT);
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
    do {
        ++radIterations;
        distributeRadDown(s, p);
        distributeRadUp(s, p);
        // Don't include upwards radiation (F_) from sky layer in residual
        residual = *s.F.at(0);
        for (int i=1; i<numLayers; ++i)
            residual += *s.F.at(i) + *s.F_.at(i);
    } while (residual > radPrecision && radIterations < maxIterations);
    if (radIterations == maxIterations)
       ThrowException("Radiation budget did not converge").value(residual).
                 hint("Check your parameters:\n" + dump(p, Dump::WithHeader)).context(this);
    // Radiation flow upwards from sky layer "passed through the sky".
    // Register this radiation as absorbed by the sky
    *s.A_[0] += *s.F_.at(0);
}

void Budget::babyStep() {
    // Force a tentative, short subTimeStep to assess the rate of temperature change
    _maxDeltaT = subTimeStep*tempPrecision/babyTimeStep;
}

void Budget::checkParameters() const {
    for (auto layer : layers)
        layer->checkParameters();
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

void Budget::writeToLog() {
    logger.write(QString::number(step) + ":" + convert<QString>(subSteps) + ": " + convert<QString>(subDateTime));
    logger.write(dump(lwParam, Dump::WithHeader));
    logger.write(dump(lwState, Dump::WithHeader));
}

}
