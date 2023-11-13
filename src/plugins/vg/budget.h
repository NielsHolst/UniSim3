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
        transpirationRate, Pn, co2Injection,
        heatPipeFlux,
        heatPumpCooling, heatPumpCondensationRate,
        babyTimeStep;
    QVector<bool> heatPipesOn;
    bool writeHighRes, isHeating, isVentilating;

    int step;
    QDateTime dateTime;
    base::Logger logger;

    // Output
    int radIterations, subSteps;
    double subTimeStep, maxDeltaT, ventilationHeatLoss,
        condensationCover, transpiration, ventedWater, condensationHeatPump, heatPumpDrying,
        indoorsSensibleHeatFlux, indoorsLatentHeatFlux, coverLatentHeatFlux,
        sunParAbsorbedInCover, sunParAbsorbedInScreens, sunParHittingPlant,
        growthLightParHittingPlant, totalPar;
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
    QVector<const double*> _sunParAbsorbedInScreens;

    // Boxes
    Sky *sky;
    AverageCover *cover;
    QVector<AverageScreen*> screens;
    GrowthLights *growthLights;
    Plant *plant;
    HeatPipes *heatingActuator;
    Floor *floor;

    // Controllers etc.
    Box
        *heatingSp, *ventilationSp,
        *heatingController, *ventilationController,
        *ventilationActuator,
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
    void babyStep();
    void checkParameters() const;
    enum class Dump{WithHeader, WithoutHeader};
    QString dump(const State &s, Dump header = Dump::WithoutHeader);
    QString dump(const Parameters &p, Dump header = Dump::WithoutHeader);
};

}

#endif
