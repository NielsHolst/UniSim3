#ifndef BUDGET_H
#define BUDGET_H
#include <base/box.h>

namespace vg {

class ActuatorVentilation;
class AverageCover;
class AverageScreen;
class BudgetLayer;
class BudgetVolume;
class Floor;
class GrowthLights;
class HeatPipes;
class LayerAdjusted;
class Plant;
class Sky;

class Budget : public base::Box {
private:
    // Input
    double
        radPrecision, tempPrecision, thresholdPrecision,
        timeStep, averageHeight, coverPerGroundArea,
        outdoorsTemperature, outdoorsRh, outdoorsCo2,
        transpirationRate, Pn, co2Injection,
        heatPipeFlux,
        ventilationThreshold, ventilationCostThreshold, heatingThreshold,
        deltaVentilationControl, deltaHeatingControl;
    QVector<bool> heatPipesOn;
    bool ventilationOn;

    // Output
    int radIterations, subSteps, controlCode, actionCode;
    double maxDeltaT, ventilationHeatLoss,
        condensation, transpiration, ventedWater, latentHeatBalance,

        indoorsSensibleHeatFlux, indoorsLatentHeatFlux;

    // Volumes
    QVector<BudgetVolume*> volumes;
    BudgetVolume *outdoorsVol, *indoorsVol, *soilVol;
    double indoorsHeatCapacity, indoorsDeltaT;

    // Layers
    QVector<BudgetLayer*> layers;
    BudgetLayer
        *budgetLayerCover,
        *budgetLayerPlant,
        *budgetLayerHeatPipes;
    int numLayers;
    Sky *sky;
    AverageCover *cover;
    QVector<AverageScreen*> screens;
    GrowthLights *growthLights;
    HeatPipes *heatPipes;
    Plant *plant;
    Floor *floor;

    // State
    struct State {
        QVector<double *> E, E_, F, F_, A, A_;
        void init();
    };
    State swState, lwState, parState;
    enum class Control{CarryOn, OnSetpointVentilation, OnSetpointHeating, GreenhouseTooHot, GreenhouseTooCold, NeedlessHeating, NeedlessCooling};
    Control control;
    bool greenhouseTooHumid, tooCostlyVentilation;
    enum class Action{CarryOn, IncreaseVentilation, DecreaseVentilation, IncreaseHeating, DecreaseHeating};
    Action action;

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
private:
    void updateSubStep(double subTimeStep, UpdateOption option);
    void updateLwEmission();
    void updateNetRadiation();
    void updateWaterBalance(double subTimeStep);
    void applyLatentHeat();
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
    void decreaseVentilation();
    void increaseHeating();
    void decreaseHeating();
    void extraVentilation();
};

}

#endif
