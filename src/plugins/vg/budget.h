/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BUDGET_H
#define BUDGET_H
#include <base/box.h>
#include <base/logger.h>

namespace vg {

class ActuatorVentilation;
class AverageCover;
class AverageScreen;
class BudgetLayer;
class BudgetLayerCover;
class BudgetVolume;
class Floor;
class GrowthLights;
class HeatPipes;
class Layer;
class Plant;
class Sky;

class Budget : public base::Box {
private:
    // Input
    double
        radPrecision, tempPrecision,
        timeStep, averageHeight, groundArea, coverPerGroundArea,
        outdoorsTemperature, outdoorsRh, outdoorsCo2,
        transpirationRate, humidificationRate, Pn, co2Injection,
        heatPipeFlux,
        heatPumpCooling, heatPumpCondensationRate,
        padAndFanCooling, padAndFanVapourFlux,
        babyTimeStep;
    QVector<bool> heatPipesOn;
    bool writeHighRes, writeLog, isHeating, isVentilating, controlClimate;

    int step;
    QDateTime dateTime;
    base::Logger logger;

    // Output
    int radIterations, subSteps;
    double subTimeStep, maxDeltaT, ventilationHeatLoss,
        condensationCover, transpiration, ventedWater, condensationHeatPump, heatPumpDrying,
        indoorsSensibleHeatFlux, indoorsLatentHeatFlux, coverLatentHeatFlux,
        sunParAbsorbedInCover, sunParAbsorbedInScreens, sunParHittingPlant,
        growthLightParHittingPlant, totalPar,
        coverConductance;
    QDateTime subDateTime;

    // Volumes
    QVector<BudgetVolume*> volumes;
    BudgetVolume *outdoorsVol, *indoorsVol, *soilVol;
    double indoorsHeatCapacity, indoorsDeltaT;

    // Layers
    QVector<BudgetLayer*> layers;
    BudgetLayerCover
        *budgetLayerCover;
    BudgetLayer
        *budgetLayerSky,
        *budgetLayerGrowthLights,
        *budgetLayerPlant,
        *budgetLayerHeatPipes;
    int numLayers;

    // Boxes
    Sky *sky;
    AverageCover *cover;
    QVector<AverageScreen*> screens;
    GrowthLights *growthLights;
    Plant *plant;
    HeatPipes *heatingActuator;
    Floor *floor;

    // Controllers etc.
    QVector<Box*> handheldBoxes;
    Box
//        *heatingSp, *heatingController,
//        *ventilationSp, *ventilationController, *ventilationActuator,
//        *heatPumpsSp, *heatPumpsController, *heatPumpsActuator,
        *outputWriter;

    // State
    struct State {
        QVector<double *> E, E_, F, F_, A, A_;
        void init();
    };
    State swState, lwState, parState;
    double _maxDeltaT;

    // Parameters
    struct Parameters {
        QVector<const double *> a, a_, r, r_, t, t_;
    };
    Parameters swParam, lwParam;

    // Actuators
//    ActuatorVentilation *actuatorVentilation;
    const double *ventilationRate;

    // Methods
    enum class UpdateOption{IncludeSwPar, ExcludeSwPar};
    void addVolumes();
    void addLayers();
    void addState();
    void addParameters();
public:
    Budget(QString name, base::Box *parent);
    void amend();
    void initialize();
    void reset();
    void update();
    void cleanup();
private:
    void updateSubStep(double subTimeStep, UpdateOption option);
    void updateLwEmission();
    void updateNetRadiation();
    void updateWaterBalance(double subTimeStep);
    void updateInSubSteps();
    void updateCo2();
//    void resetState();
    void distributeRadDown(State &s, const Parameters &p);
    void distributeRadUp(State &s, const Parameters &p);
    void distributeRadiation(State &s, const Parameters &p);
    void updateConvection();
    void updateDeltaT(double timeStep);
    void applyDeltaT();
    void babyStep();
    void checkParameters() const;
    enum class Dump{WithHeader, WithoutHeader};
    QString dump(const State &s, Dump header = Dump::WithoutHeader);
    QString dump(const Parameters &p, Dump header = Dump::WithoutHeader);
    void writeToLog();
};

}

#endif
