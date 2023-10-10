#ifndef BUDGET_H
#define BUDGET_H
#include <base/box.h>
//#include <base/logger.h>

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
        tempSetpointPrecision,
        timeStep, averageHeight, coverPerGroundArea,
        outdoorsTemperature, outdoorsRh, outdoorsCo2,
        transpirationRate, Pn, co2Injection,
        heatPipeFlux,
        ventilationThreshold, ventilationCostThreshold, heatingThreshold,
        ventilationNeed,
        deltaVentControl, deltaVentControlRelative, deltaHeatingControl,
        babyTimeStep;
    QVector<bool> heatPipesOn;
    bool ventilationOn;

    int step;
    QDateTime dateTime;
//    base::Logger logger;

    // Output
    int radIterations, subSteps, statusCode, actionCode;
    double maxDeltaT, ventilationHeatLoss,
        condensation, transpiration, ventedWater,
        indoorsSensibleHeatFlux, indoorsLatentHeatFlux, coverLatentHeatFlux;

    // Volumes
    QVector<BudgetVolume*> volumes;
    BudgetVolume *outdoorsVol, *indoorsVol, *soilVol;
    double indoorsHeatCapacity, indoorsDeltaT;

    // Layers
    QVector<BudgetLayer*> layers;
    BudgetLayerCover
        *budgetLayerCover;
    BudgetLayer
        *budgetLayerPlant,
        *budgetLayerHeatPipes;
    int numLayers;
    Sky *sky;
    AverageCover *cover;
    QVector<AverageScreen*> screens;
    GrowthLights *growthLights;
    Plant *plant;
    HeatPipes *heatPipes;
    Floor *floor;

    // State
    struct State {
        QVector<double *> E, E_, F, F_, A, A_;
        void init();
    };
    State swState, lwState, parState;
    enum class Status{WithinSetpoints, OnSetpointVentilation, OnSetpointHeating,
                                 GreenhouseTooHot, GreenhouseTooCold,
                                 NeedlessHeating, NeedlessCooling};
    Status status;

    bool tooCostlyVentilation;
    enum class Action{CarryOn, IncreaseVentilation, DecreaseVentilation, IncreaseHeating, DecreaseHeating};
    Action action;
    double _maxDeltaT, _subTimeStep;

    // Parameters
    struct Parameters {
        QVector<const double *> a, a_, r, r_, t, t_;
    };
    Parameters swParam, lwParam;

    // Actuators
    ActuatorVentilation *actuatorVentilation;
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
    void updateLayersAndVolumes();
    void updateCo2();
    void resetState();
    void distributeRadDown(State &s, const Parameters &p);
    void distributeRadUp(State &s, const Parameters &p);
    void distributeRadiation(State &s, const Parameters &p);
    void updateConvection();
    void updateDeltaT(double timeStep);
    void applyDeltaT();
    void saveForRollBack();
    void rollBack();
    void diagnoseControl();
    void exertControl();
    void increaseVentilation();
    void increaseVentilationIfNeeded();
    void decreaseVentilation();
    void increaseHeating();
    void decreaseHeating();
    void extraVentilation();
    void stopHeating();
    void stopVentilation();
    void babyStep();
    void checkParameters() const;
    enum class Dump{WithHeader, WithoutHeader};
    QString dump(const State &s, Dump header = Dump::WithoutHeader);
    QString dump(const Parameters &p, Dump header = Dump::WithoutHeader);
};

}

#endif
