/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef OUTPUTS_H
#define OUTPUTS_H

#include <base/box.h>


namespace vg {

class Outputs : public base::Box
{
public:
    Outputs(QString name, Box *parent);
    void amend();
    void reset();
    void update();
private:
    // Inputs
    bool onlyNumbers;
    double sunPar, growthLightsPar, cropCoverage;
    // Outputs
    double
        skyT,
        outdoorsT,
        outdoorsRh,
        outdoorsLight,
        outdoorsPar,
        outdoorsWindSpeed,
        soilT,
        coverT,
        screen1T,
        screen2T,
        screen3T,
        pipeT,
        leafT,
        floorT,
        indoorsT,
        indoorsRh,
        indoorsTotalPar,
        indoorsGrowthLightsPar,
        indoorsSunPar,
        indoorsCo2,
        Uinside,
        Uoutside,
        spHeatingTemperature,
        spVentilationTemperature,
        spRhMax,
        spRhMin,
        spCrackVentilation,
        spScreenEnergyThreshold1,
        spScreenEnergyThreshold2,
        spScreenShadeThreshold1,
        spScreenShadeThreshold2,
        spScreenFixed1,
        spScreenFixed2,
        spScreenEnergyThresholdBand,
        spScreenShadeThresholdBand,
        spScreenCrack,
        spCo2,
        actHeatingPower,
        actVentilation,
        airTransmissivityScreens,
        airFluxTotal,
        airFluxLeakage,
        airFluxVentilation,
        airFluxPower,
        actPipeTempIn1,
        actPipeTempIn2,
        actPipeTempOut1,
        actPipeTempOut2,
        actHeatSink,
        actLight1,
        actLight2,
        actLight3,
        actLightPower,
        actLightPar,
        actScreen1,
        actScreen2,
        actScreen3,
        actScreen1HeatCapacity,
        actCo2,
        vapourFluxTranspiration,
        vapourFluxCondensationCrop,
        vapourFluxCondensationCover,
        vapourFluxCondensationdScreens,
        vapourFluxVentilation,
        grossAssimilationRate,
        netAssimilationRate,
        respirationRate,
        netGrowthRate,
        netGrowthRateLeaf,
        lightUseEfficiency,
        totalIrradiation,
        totalSkyAbsorbed,
        totalSkySwAbsorbed,
        totalSkyLwAbsorbed,
        totalVentilationEnergy,
        totalConvectionEnergy,
        totalSoilEnergy,
        totalHeatingEnergy,
        totalHeatSink,
        totalGrowthLightEnergy,
        totalParAbsorbed,
        totalCo2;
    int thermostateUpdates;
    QString thermostatState, thermostatAction, thermostatSolution;
};
} //namespace


#endif
