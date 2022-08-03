/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HEAT_TRANSFER_LAYER_BASE_H
#define HEAT_TRANSFER_LAYER_BASE_H
#include "heat_transfer_layer_parameters.h"

namespace vg {

class HeatTransferLayerBase : public HeatTransferLayerParameters
{
    friend class EnergyBudget;
public:
    HeatTransferLayerBase(QString name, Box *parent);
protected:
    // Inputs
    double
        parFluxDown, parFluxUp,
        swFluxDown, swFluxUp,
        convectiveInflux, conductiveInflux,
        condensationRate,
        maxTemperatureRateOfChange, unusedInflux,
        timeStep;
    // Outputs
    double
        parAbsorbedFromAbove, parAbsorbedFromBelow, parAbsorbed,
        swAbsorbedFromAbove, swAbsorbedFromBelow, swAbsorbed,
        lwAbsorbedFromAbove, lwAbsorbedFromBelow, lwAbsorbed,
        lwFluxDown, lwFluxUp,
        absorbed, latentHeat,
        temperature, temperatureTop, temperatureBottom;
    // Methods
    virtual void updateLwEmission();
    void updateTemperature();
};

} //namespace


#endif
