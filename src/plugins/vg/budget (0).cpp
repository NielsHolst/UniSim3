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

    QString fix(double x, int dec=2) {
        return QString("%1").arg(x, 0, 'f', dec);
    }

    const struct {
        QStringList portNames = {
            "swEmissionTop", "swEmissionBottom",
            "swAbsorbedTop", "swAbsorbedBottom",
            "lwEmissionTop", "lwEmissionBottom",
            "lwAbsorbedTop", "lwAbsorbedBottom",
            "convectionTop", "convectionBottom",
            "condensation"
        };
        QVector<int> sign = {
           -1, -1,
            1,  1,
           -1, -1,
            1,  1,
            1,  1,
            1
        };
    } flux;
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
    Input(isSkippingOutput).imports("OutputSelector::*[isSkipping]");
    Input(controlClimate).equals(true).help("Should climate be controlled according to setpoints?");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Input(averageHeight).imports("gh/geometry[averageHeight]");
    Input(groundArea).imports("gh/geometry[groundArea]");
    Input(coverPerGroundArea).imports("gh/geometry[coverPerGroundArea]");
    Input(outdoorsTemperature).imports("outdoors[temperature]");
    Input(outdoorsRh).imports("outdoors[rh]");
    Input(outdoorsCo2).imports("outdoors[co2]");
    Input(babyTimeStep).equals(1.).unit("s").help("Length of first time step after climate control action");
    Input(step).imports("/.[step]");
    Input(dateTime).imports("calendar[dateTime]");

    Output(subDateTime).help("Date time within integration time step");
    Output(subTimeStep).unit("s").help("Length of integration sub-time step");
    Output(subSteps).help("Number of sub-steps taken to resolve the whole budget");
    Output(radIterations).help("Number of iterations taken to resolve radiation budget");
    Output(maxDeltaT).unit("K").help("Max. temperature change in a sub-step");

    Output(wfTranspiration).unit("L/m2/h").help("Water flux by plant transpiration");
    Output(wfHumidification).unit("L/m2/h").help("Water flux by humidifiers (fogging)");
    Output(wfVentilation).unit("L/m2/h").help("Water flux by ventilation");
    Output(wfCover).unit("L/m2/h").help("Water flux by condensation on cover");
    Output(wfHeatPump).unit("L/m2/h").help("Water flux by heat pumps");
    Output(wfPadAndFan).unit("L/m2/h").help("Water flux by pad and fan");

    Output(ventilationHeatLoss).unit("W/m2").help("Sensible heat lost by ventilation; positive if outdoors warmer than indoors");
    Output(indoorsSensibleHeatFlux).unit("W/m2").help("Rate of change in indoors air sensible heat");
    Output(indoorsLatentHeatFlux).unit("W/m2").help("Rate of change in indoors air latent heat");
    Output(coverLatentHeatFlux).unit("W/m2").help("Rate of condensation heat influx to cover");
    Output(sunParHittingPlant).unit("&micro;mol/m2/s").help("Sunlight PAR hitting plant canopy");
    Output(growthLightParHittingPlant).unit("&micro;mol/m2/s").help("Growth light PAR hitting plant canopy");
    Output(totalPar).unit("&micro;mol/m2/s").help("Total PAR hitting plant canopy");
    Output(coverConductance).unit("/s").help("Conductance for condensation on inside of cover");
    Output(checkSum).help("Sum of energy flows; must approximate zero");
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
            port("initTemperature").imports("outdoors[temperature]").
            port("temperature").imports("outdoors[temperature]").
            port("initRh").imports("outdoors[rh]").
            port("rh").imports("outdoors[rh]").
            port("ah").imports("outdoors[ah]").
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
    sky             = findOne<Sky*>("/sky");
    cover           = findOne<AverageCover*>("shelter/layers/cover");
    screens         = findMany<AverageScreen*>("shelter/layers/screens/*");
    growthLights    = findMaybeOne<GrowthLights*>("actuators/growthLights");
    heatingActuator = findMaybeOne<HeatPipes*>("actuators/heatPipes");
    plant           = findMaybeOne<Plant*>("gh/plant");
    floor           = findOne<Floor*>("gh/floor");

    // Build layers as children
    BoxBuilder builder(this);
    builder.
        box("BudgetLayerSky").name("sky").
            port("temperature").imports("/sky[temperature]").
            port("swEmissionBottom").imports("outdoors[radiation]").
            port("parEmissionBottom").imports("outdoors[par]").
        endbox().
        box("BudgetLayerCover").name("cover").
//            port("initTemperature").computes("(2*budget/indoors[initTemperature] + budget/outdoorsVol[initTemperature]) / 3").
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
    if (heatingActuator) {
        builder.
            box("BudgetLayerHeatPipes").name("heatPipes").
            endbox();
    }
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
    else
        budgetLayerGrowthLights = nullptr;

    // Attach plant
    if (plant) {
        budgetLayerPlant = findOne<BudgetLayer*>("./plant");
        budgetLayerPlant->attach(plant, indoorsVol, indoorsVol);
        layers << budgetLayerPlant;
    }
    else
        budgetLayerPlant = nullptr;

    // Attach heat pipes
    if (heatingActuator) {
        budgetLayerHeatPipes = findOne<BudgetLayer*>("./heatPipes");
        budgetLayerHeatPipes->attach(heatingActuator, indoorsVol, indoorsVol);
        layers << budgetLayerHeatPipes;
    }
    else
        budgetLayerHeatPipes = nullptr;

    // Attach floor
    budgetLayerFloor = findOne<BudgetLayer*>("./floor");
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
        "setpoints/ventilation|"
        "setpoints/heating|"
        "setpoints/heatPumps|"
        "setPoints/padAndFans|"
        "setPoints/humidification|"

        "controllers/desiredMinTemperature|"
        "controllers/ventilation|"
        "controllers/heatPipes|"
        "controllers/heatPumps|"
        "controllers/padAndFans|"
        "controllers/humidification|"

        "actuators/ventilation|"
        "actuators/heatPipes|"
        "actuators/heatPumps|"
        "actuators/padAndFans|"
        "actuators/humidifiers"
    );

    // Set pointers to direct inputs
    Box *actuatorVentilation = findMaybeOne<Box*>("actuators/ventilation");
    in.ventilationRate = actuatorVentilation ? actuatorVentilation->port("value")->valuePtr<double>() :
                                               findOne<Port*>("shelter/leakage[value]")->valuePtr<double>();
    in.transpirationRate = ptrOrZero("gh/plant[transpiration]");
    in.humidificationRate = ptrOrZero("actuators/humidifiers[vapourFlux]");
    in.Pn = ptrOrZero("gh/plant[Pn]");
    in.co2Injection = ptrOrZero("gh/actuators/co2[value]");
    in.heatPumpCooling = ptrOrZero("actuators/heatPumps[cooling]");
    in.heatPumpCondensationRate = ptrOrZero("actuators/heatPumps[condensation]");
    in.padAndFanCooling = ptrOrZero("actuators/padAndFans[cooling]");
    in.padAndFanVapourFlux = ptrOrZero("actuators/padAndFans[vapourFlux]");

    // Find writer for detailed output
    outputWriter = findMaybeOne<Box*>("outputWriter");
    if (!outputWriter)
        writeHighRes = false;
}

const double * Budget::ptrOrZero(QString path) {
    Port *port = findMaybeOne<Port*>(path);
    return port ? port->valuePtr<double>() : &(in.zero);
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
        logger.open(environment().outputFileNamePath("vg.log"));
}

namespace {
    double guardedRatio(double a, double b) {
        return (b == 0.) ? 0. : a/b;
    }
}

void Budget::update() {
    checkParameters();
    const double
            indoorsTemp0 = indoorsVol->temperature,
            indoorsAh0   = indoorsVol->ah;
    updateInSubSteps();
    updateCo2();
    updateCheckSum();

    indoorsSensibleHeatFlux = (indoorsVol->temperature - indoorsTemp0)*RhoAir*CpAir*averageHeight/timeStep;
    indoorsLatentHeatFlux   = (indoorsVol->ah          - indoorsAh0)  *LHe         *averageHeight/timeStep;

    // Approximate outputs
    growthLightParHittingPlant = budgetLayerGrowthLights ? budgetLayerGrowthLights->parEmissionBottom : 0.;
    // If no plant then use floor instead
    BudgetLayer *refLayer = budgetLayerPlant ? budgetLayerPlant : budgetLayerFloor;
    totalPar = guardedRatio(refLayer->parAbsorbedTop   , refLayer->attachedLayer->swAbsorptivityTop   ) +
               guardedRatio(refLayer->parAbsorbedBottom, refLayer->attachedLayer->swAbsorptivityBottom);
    sunParHittingPlant = std::max(totalPar - growthLightParHittingPlant, 0.);

    // Log as needed
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
    // Water fluxes to be summed
    wfTranspiration =
    wfHumidification =
    wfVentilation   =
    wfCover  =
    wfHeatPump =
    wfPadAndFan = 0.;
    // Loop through minor time steps
    while (lt(timePassed, timeStep) && subSteps < maxSubSteps) {
        subTimeStep = std::min(subTimeStep*tempPrecision/_maxDeltaT, timeStep - timePassed);

        // Update deltaT for all volumes and layers, except plant
        updateSubStep(subTimeStep, (timePassed == 0.) ? UpdateOption::IncludeSwPar : UpdateOption::ExcludeSwPar);

        // Update plant
        if (plant)
            plant->updateByRadiation(budgetLayerPlant->netRadiation,
                                     budgetLayerPlant->parAbsorbedTop +
                                     budgetLayerPlant->parAbsorbedBottom);

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
            writeToLog();
        }

        // Count sub-steps with major simulation time step
        ++subSteps;
    }
    // Convert water fluxes from kg/m2 to L/m2/h
    const double c = 3600./timeStep;
    wfTranspiration  *= c;
    wfHumidification *= c;
    wfVentilation    *= c;
    wfCover          *= c;
    wfHeatPump       *= c;
    wfPadAndFan      *= c;
    // Too many minor steps?
    if (subSteps == maxSubSteps)
        dialog().error("On" + convert<QString>(dateTime) + ": Energy budget did not converge");
}

void Budget::updateSubStep(double subTimeStep, UpdateOption option) {
    // Volume heat fluxes to be summed from neighbouring layers
    for (BudgetVolume *volume : volumes)
        volume->heatInflux = 0.;

    // Reset to find maximum temperature change during sub-step
    _maxDeltaT = 0.;
    // Update radiation budget
    updateLwEmission();
    if (option == UpdateOption::IncludeSwPar) {        swState.init();
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
    for (BudgetLayer *layer : layers) {
        layer->updateConvection();
    }
}

void Budget::updateDeltaT(double timeStep) {
    for (BudgetLayer *layer : layers) {
        double deltaT = layer->updateDeltaT(timeStep);
        if (fabs(deltaT) > _maxDeltaT)
            _maxDeltaT = fabs(deltaT);
    }
    double propVentilation   = 1. - exp(-(*in.ventilationRate)/3600.*timeStep),
           ventilationDeltaT = (outdoorsTemperature - indoorsVol->temperature)*propVentilation;
//           ventilationDeltaT = std::max(outdoorsTemperature - indoorsVol->temperature, 0.)*propVentilation;
    // (disallow heating by warmer outside air)
    indoorsDeltaT = (indoorsVol->heatInflux - (*in.heatPumpCooling + *in.padAndFanCooling))*timeStep/indoorsHeatCapacity + ventilationDeltaT;
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
    // All in kg/m3
    double
        deltaAh,
        evapotranspiration,
        condensation,
        ventilation;
};

WaterIntegration waterIntegration(
        double dt,  // time step
        double ah0, // indoors ah
        double ET,  // evapotranspiration
        double c,   // cover condensation rate
        double C,   // sah of cover
        double v,   // ventilation rate
        double V)   // outdoors ah
{
    // No condensation possible?
    if (ah0 < C)
        c = 0.;
    // Only evapotranspiration
    if (fabs(c+v) < 1e-9) {
        return WaterIntegration {ET*dt, ET*dt, 0., 0.};
    }
    // Else integrate
    const double
        tau          = exp(-(c+v)*dt),
        ah1          = (c + v != 0.) ? (ET + c*C + v*V - tau*(c*C - ah0*(c + v) + ET + v*V))/(c + v) : ah0 + ET*dt,
        deltaAh      = ah1 - ah0,
        avgAh        = (ah0 + ah1)/2.;

    // Break down water budget
    WaterIntegration w;
    w.deltaAh = deltaAh;
    w.evapotranspiration = ET*dt;

    const double
        C1 = c*(avgAh - C),
        V1 = v*(avgAh - V),
        k = (ET - deltaAh/dt) / (C1 + V1);
    w.condensation = k*C1;
    w.ventilation  = k*V1;
    return w;
}

inline double g(double volumeVirtT, double coverVirtT) {
    return (volumeVirtT > coverVirtT) ? 1.64e-3*pow(volumeVirtT - coverVirtT, 0.333) : 0.;
}

}

void Budget::updateWaterBalance(double timeStep) {
    const double
       indoorsAh     = indoorsVol->ah,
       outdoorsAh    = outdoorsVol->ah,
       coverSah      = sah(budgetLayerCover->temperature),
       indoorsVirtT  = virtualTemperatureFromAh(indoorsVol->temperature,       indoorsAh),
       coverVirtT    = virtualTemperatureFromAh(budgetLayerCover->temperature, indoorsAh),
       v             = (*in.ventilationRate)/3600.;
    coverConductance = coverPerGroundArea*g(indoorsVirtT, coverVirtT);

    WaterIntegration w = waterIntegration(
        timeStep,
        indoorsAh,
        (*in.transpirationRate + *in.humidificationRate + *in.padAndFanVapourFlux - *in.heatPumpCondensationRate)/averageHeight,
        coverConductance,
        coverSah,
        v,
        outdoorsAh
    );

    // Net condensation and ventilation
    const double
        // kg/m2 = kg/m3 * m
        d_condensationCover    = w.condensation*averageHeight,
        d_ventedWater          = w.ventilation*averageHeight;

    // Evapotranspiration
    double
        // kg/m2 = kg/m2/s * s
        d_transpiration        = *in.transpirationRate*timeStep,
        d_humidification       = *in.humidificationRate*timeStep,
        d_padAndFanVapourFlux  = *in.padAndFanVapourFlux*timeStep,
        d_heatPumpCondensation = *in.heatPumpCondensationRate*timeStep;

    // Correction of evapotranspiration sum
    const double
        d_sum = d_transpiration + d_humidification + d_padAndFanVapourFlux - d_heatPumpCondensation,
        k = (fabs(d_sum) < 1e-6) ? 1. : w.evapotranspiration/d_sum;
    d_transpiration        *= k;
    d_humidification       *= k;
    d_padAndFanVapourFlux  *= k;
    d_heatPumpCondensation *= k;

    // Outputs
    wfTranspiration  += d_transpiration;
    wfHumidification += d_humidification;
    wfVentilation    -= d_ventedWater;
    wfCover          -= d_condensationCover;
    wfHeatPump       -= d_heatPumpCondensation;
    wfPadAndFan      += d_padAndFanVapourFlux;

    // Indoors state update
    indoorsVol->ah   += w.deltaAh;
    const double indoorsSah = sah(indoorsVol->temperature);
    if (indoorsVol->ah > indoorsSah)
        indoorsVol->ah = indoorsSah;
    indoorsVol->updateRh();

    // Outside latent heat
    const double
       outdoorsVirtT       = virtualTemperatureFromAh(outdoorsTemperature, outdoorsAh),
       c                   = coverPerGroundArea*g(outdoorsVirtT, coverVirtT),
       outsideCondensation = std::max(c*(outdoorsAh - coverSah)*timeStep, 0.),
       coverCondensation   = d_condensationCover + outsideCondensation;

    // Update cover temperature
    coverLatentHeatFlux = LHe*coverCondensation/timeStep;
    double deltaT = budgetLayerCover->updateDeltaTByCondensation(coverCondensation);
    if (fabs(deltaT) > _maxDeltaT)
        _maxDeltaT = fabs(deltaT);
}

void Budget::updateCo2() {
    double
        indoorsCo2  = 1.829e-3*indoorsVol->co2,
        outdoorsCo2 = 1.829e-3*Budget::outdoorsCo2,
        injection   = (*in.co2Injection)/3600.,
        fixation    = (*in.Pn)*44.01e-6/averageHeight,
        c           = injection - fixation,
        v           = (*in.ventilationRate)/3600.;

    if (eqZero(v)) {
        indoorsCo2 += c*timeStep;
    }
    else {
        indoorsCo2 = integrate(timeStep, indoorsCo2, c, v, outdoorsCo2);
    }
    indoorsVol->co2 = indoorsCo2/1.829e-3;
}

void Budget::updateCheckSum() {
    double
        sumLayers = 0.,
        sumVolumes = 0.;
    for (BudgetLayer *layer : layers) {
        int j = 0;
        for (QString portName : flux.portNames) {
            double value = flux.sign.at(j++)*layer->port(portName)->value<double>();
            sumLayers += value;
        }
    }
    for (auto volume : volumes) {
        sumVolumes += volume->port("heatInflux")->value<double>();
    }
    checkSum = sumLayers + sumVolumes;
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
            << fix(*s.E.at(i),3) << "\t"
            << fix(*s.E_.at(i),3) << "\t"
            << fix(*s.F.at(i),3) << "\t"
            << fix(*s.F_.at(i),3) << "\t"
            << fix(*s.A.at(i),3) << "\t"
            << fix(*s.A_.at(i),3) << "\t"
            << fix(layers.at(i)->temperature) << "\n";
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
            << fix(*p.a.at(i),3) << "\t"
            << fix(*p.a_.at(i),3) << "\t"
            << fix(*p.r.at(i),3) << "\t"
            << fix(*p.r_.at(i),3) << "\t"
            << fix(*p.t.at(i),3) << "\t"
            << fix(*p.t_.at(i),3) << "\n";
    }
    return string;
}

QString Budget::dumpVolumes(Dump header) {
    QString string;
    QTextStream result(&string);
    if (header == Dump::WithHeader)
        result << "i\tvolume\tinflux\tT\n";
    double sum = 0.;
    int i = 0;
    for (auto volume : volumes) {
        double heatInflux  = volume->port("heatInflux")->value<double>(),
               temperature = volume->port("temperature")->value<double>();
        result
            << i++ << "\t"
            << volume->name() << "\t"
            << fix(heatInflux) << "\t"
            << fix(temperature) << "\n";
        sum += heatInflux;
    }
    result << "\tsum\t" << fix(sum) << "\n";
    return string;
}

QString Budget::dumpLayers() {

    QString string;
    QTextStream result(&string);
    // Write header
    result << "i\tlayer\t" << flux.portNames.join("\t") << "\tsum\n";
    // Write layers with sum
    int i = 0;
    for (BudgetLayer *layer : layers) {
        result
            << i++ << "\t"
            << layer->name() << "\t";
        double sum = 0.;
        int j = 0;
        for (QString portName : flux.portNames) {
            double value = flux.sign.at(j++)*layer->port(portName)->value<double>();
            result << fix(value) << "\t";
            sum += value;
        }
        result << fix(sum) << "\n";
    }
    // Write column sums
    result << "\tsum\t";
    double total = 0.;
    int j = 0;
    for (QString portName : flux.portNames) {
        double sum = 0.;
        for (BudgetLayer *layer : layers) {
            sum += flux.sign.at(j)*layer->port(portName)->value<double>();
        }
        result << fix(sum) << "\t";
        total += sum;
        j++;
    }
    result << fix(total) << "\n";
    return string;
}

void Budget::writeToLog() {
    if (!isSkippingOutput) {
        logger.write(QString::number(step) + "\t" + convert<QString>(subSteps) + "\t" + convert<QString>(subDateTime));
        logger.write(dump(lwParam, Dump::WithHeader));
        logger.write(dump(lwState, Dump::WithHeader));
        logger.write(dumpVolumes(Dump::WithHeader));
        logger.write(dumpLayers());
    }
}

}
