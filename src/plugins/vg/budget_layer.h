/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BUDGET_LAYER_H
#define BUDGET_LAYER_H
#include <base/box.h>

namespace vg {

class LayerAdjusted ;

class BudgetLayer : public base::Box {
public:
    BudgetLayer(QString name, base::Box *parent);
    ~BudgetLayer();
    void attach(const LayerAdjusted *layer);
    void reset();
    void update();
    void updateLwEmission();
    void transferEmissionsToFlows();
private:
    // Inputs
    bool updateLwFromTemperature;
    // Outputs
    double
        temperature,

        swEmissionTop, swEmissionBottom,
        lwEmissionTop, lwEmissionBottom,
        parEmissionTop, parEmissionBottom,

        swFlowTop, swFlowBottom,
        lwFlowTop, lwFlowBottom,
        parFlowTop, parFlowBottom,

        swAbsorbedTop, swAbsorbedBottom,
        lwAbsorbedTop, lwAbsorbedBottom,
        parAbsorbedTop, parAbsorbedBottom,

        convectiveFluxTop, convectiveFluxBottom;

    // Data
    const LayerAdjusted *attachedLayer;

    struct Parameters {
        const double &a, &r, &t, &a_, &r_, &t_;
    };
    const Parameters *psw, *plw, &sw, &lw;

    struct State {
        double &E, &E_, &F, &F_, &A, &A_;
    };
    State swS, lwS, parS;

    // Friends
    friend class Budget;
};

}

#endif
