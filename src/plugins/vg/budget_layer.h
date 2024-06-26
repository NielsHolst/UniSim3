/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BUDGET_LAYER_H
#define BUDGET_LAYER_H
#include <base/box.h>

namespace vg {

class BudgetVolume;
class Layer;

class BudgetLayer : public base::Box {
public:
    BudgetLayer(QString name, base::Box *parent);
    void attach(const Layer *layer, BudgetVolume *top, BudgetVolume *bottom);
    void reset() final;
    void updateConvection();
    virtual double updateCondensation();
    virtual void updateLwEmission();
    virtual double updateDeltaT(double timeStep);
    void updateDeltaTEnergy();
    void updateNetRadiation();
    void checkParameters() const;
private:
    // Inputs
    double
        initTemperature;
    // Outputs
    double
        swEmissionTop, swEmissionBottom,
        // (lw is protected)
        parEmissionTop, parEmissionBottom,

        swFlowTop, swFlowBottom,
        lwFlowTop, lwFlowBottom,
        parFlowTop, parFlowBottom,

        swAbsorbedTop, swAbsorbedBottom,
        convectionTop, convectionBottom,
        radiationDeltaT, convectionDeltaT;
    // Data
    double rollBackTemperature;
protected:
    // Outputs
    double
        temperature,
        lwAbsorbedTop, lwAbsorbedBottom,
        lwEmissionTop, lwEmissionBottom,
        parAbsorbedTop, parAbsorbedBottom,
        netRadiation, netConvection,
        condensation, totalDeltaT, totalDeltaTEnergy;
    // Data
    const double *emissivityTop, *emissivityBottom,
        *Utop, *Ubottom,
        *heatCapacity,
        *temperatureVolumeTop, *temperatureVolumeBottom,
        *state;
private:
    // Data
    const Layer *attachedLayer;
    BudgetVolume *volumeTop, *volumeBottom;
    bool
        lwEmissionTopUpdatedExternally, lwEmissionBottomUpdatedExternally,
        convectionTopUpdatedExternally, convectionBottomUpdatedExternally;
    // Friends
    friend class Budget;
};

}

#endif
