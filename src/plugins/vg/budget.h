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
        timeStep, averageHeight,
        outdoorsTemperature, outdoorsRh,
        transpirationRate,
        ventilationThreshold, heatingThreshold,
        deltaVentilationControl, deltaHeatingControl;
    QVector<bool> heatPipesOn;
    bool ventilationOn;
    // Output
    int radIterations, subSteps, controlCode, actionCode;
    double maxDeltaT, advectionDeltaT,
        indoorsDeltaAh;
    // Volumes
    QVector<BudgetVolume*> volumes;
    BudgetVolume *outdoorsVol, *indoorsVol, *soilVol;
    double indoorsHeatCapacity, indoorsDeltaT;
    // Layers
    QVector<BudgetLayer*> layers;
    BudgetLayer *budgetLayerHeatPipes;
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
    enum class Action{CarryOn, IncreaseVentilation, DecreaseVentilation, IncreaseHeating, DecreaseHeating};
    Action action;
    // Data
    double indoorsAh;
    // Parameters
    struct Parameters {
        QVector<const double *> a, a_, r, r_, t, t_;
    };
    Parameters swParam, lwParam;
    // Actuators
    ActuatorVentilation *actuatorVentilation;
    const double *ventilationRate;
    // Methods
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
    void updateSubStep(double subTimeStep);
    void updateLwEmission();
    void updateLayersAndVolumes();
    void resetState();
    void distributeRadDown(State &s, const Parameters &p);
    void distributeRadUp(State &s, const Parameters &p);
    void distributeRadiation(State &s, const Parameters &p);
    void updateConvection();
    void updateDeltaT(double timeStep);
    void updateDeltaAh(double timeStep);
    void applyDeltaT();
    void applyDeltaAh();
    void saveForRollBack();
    void rollBack();
    void diagnoseControl();
    void exertControl();
    void increaseVentilation();
    void decreaseVentilation();
    void increaseHeating();
    void decreaseHeating();
};

}

#endif
