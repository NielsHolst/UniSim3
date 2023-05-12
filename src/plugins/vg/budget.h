#ifndef BUDGET_H
#define BUDGET_H
#include <base/box.h>

namespace vg {

class BudgetLayer;
class BudgetVolume;
class Sky;
class LayerAdjusted;
class AverageCover;
class AverageScreen;
class Plant;
class Floor;

class Budget : public base::Box {
private:
    // Input
    double precision;
    // Output
    int iterations;
    // Volumes
    BudgetVolume *outdoorsVol, *indoorsVol, *soilVol;
    // Layers
    QVector<BudgetLayer*> layers;
    int numLayers;
    Sky *sky;
    AverageCover *cover;
    QVector<AverageScreen*> screens;
    QVector<Box*> lights,
                  heatPipes;
    Plant *plant;
    Floor *floor;
    // State
    struct State {
        QVector<double *> E, E_, F, F_, A, A_;
        void init();
    };
    State swState, lwState, parState;
    // Parameters
    struct Parameters {
        QVector<const double *> a, a_, r, r_, t, t_;
    };
    Parameters swParam, lwParam;
    // Data
    QVector<double> deltaT;
    // Methods
    void addVolumes();
    void addLayers();
    void addState();
    void addParameters();
public:
    Budget(QString name, base::Box *parent);
    void amend();
    void reset();
    void update();
    void updateLwEmission();
    void resetState();
    void distributeRadDown(State &s, const Parameters &p);
    void distributeRadUp(State &s, const Parameters &p);
    void distributeRadiation(State &s, const Parameters &p);
    void updateDeltaT();
};

}

#endif
