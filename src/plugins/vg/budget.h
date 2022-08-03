/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BUDGET_H
#define BUDGET_H

#include <base/box.h>

namespace vg {

class Budget : public base::Box
{
public:
    Budget(QString name, Box *parent);
    void initialize();
    void reset();
    void update();

private:
    // Inputs
    QDate date;
    double
        dt,
        skyIrradiationRate,
        skyRadiationAbsorbedRate, skySwRadiationAbsorbedRate, skyLwRadiationAbsorbedRate,
        ventilationEnergyRate,
        convectionEnergyRate,
        heatSinkEnergyRate,
        groundArea,
        indoorsAh, outdoorsAh,
        soilEnergyRate,
        heatingPowerUsage,
        growthLightPowerUsage,
        parAbsorbed,
        co2Flux,
        netGrowthRate;
    // Outputs
    double
        lightUseEfficiency,
        skyIrradiation,
        skyRadiationAbsorbed, skySwRadiationAbsorbed, skyLwRadiationAbsorbed,
        ventilationEnergy,
        convectionEnergy,
        heatSinkEnergy,
        latentHeatEnergy,
        soilEnergy,
        heatingEnergy,
        growthLightEnergy,
        parAbsorbedTotal,
        co2Total;
    // Data
    double _kiloHour, _kiloMol;
    QDate _prevDate;
    bool _hasResetSums;
    // Method
    void checkNewYear();

};
} //namespace


#endif
