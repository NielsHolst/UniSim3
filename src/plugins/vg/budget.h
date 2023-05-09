#ifndef BUDGET_H
#define BUDGET_H
#include <base/box.h>

namespace vg {

class BudgetLayer;
class Sky;
class LayerAdjusted;
class AverageCover;
class AverageScreen;
class Plant;
class Floor;

class Budget : public base::Box {
private:
    // Data
    QVector<BudgetLayer*> layers;
    Sky *sky;
    AverageCover *cover;
    QVector<AverageScreen*> screens;
    QVector<Box*> lights,
                  heatPipes;
    Plant *plant;
    Floor *floor;

    struct State {
        QVector<const double *> E, E_;
        QVector<      double  > F, F_, A, A_;
    };
    struct Parameters {
        QVector<const double *> a, a_, r, r_, t, t_;
    };

    State swState, lwState, parState;
    Parameters swPar, lwPar;

public:
    Budget(QString name, base::Box *parent);
    void amend();
    void update();
    void updateLwEmission();
    void transferEmissionsToFlows();
    void distributeRadDown(State &s, const Parameters &p);
    void distributeRadUp(State &s, const Parameters &p);
    void distributeRadiation(State &s, const Parameters &p);
};

}

#endif
