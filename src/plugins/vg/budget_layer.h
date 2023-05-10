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
    void attach(const LayerAdjusted *layer);
    void reset();
    void update();
    virtual void updateLwEmission();
private:
    // Inputs
    double
        initTemperature;
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
    const double *emissivityTop, *emissivityBottom;
    bool lwEmissionTopUpdatedExternally, lwEmissionBottomUpdatedExternally;
    // Friends
    friend class Budget;
};

}

#endif
