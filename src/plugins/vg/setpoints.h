/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SETPOINTS_H
#define SETPOINTS_H
#include <base/box.h>

namespace vg {

class Setpoints : public base::Box
{
public:
    Setpoints(QString name, Box *parent);
    void amend();
    void reset();
    void update();
private:
    // Inputs
    double
        heatingTemperatureNormalRh,
        heatingTemperatureHighRhMargin,
        heatingTemperatureLowRhMargin,
        rhMax,
        rhMaxBand,
        rhMin,
        rhMinBand,
        ventilationTemperatureMargin,
        crackVentilationNormalTemperature,
        crackVentilationTemperatureMin,
        crackVentilationTemperatureMinBand,
        co2Capacity,
        co2Setpoint,
        co2VentilationThreshold,
        co2VentilationBand,
        chalk,
        screenEnergyThreshold1,
        screenEnergyThreshold2,
        screenEnergyThresholdBand,
        screenShadeThreshold1,
        screenShadeThreshold2,
        screenShadeThresholdBand,
        screenFixed1,
        screenFixed2,
        screenCrackAtHighRh,
        screenCrackAtHighTemperature,
        screenCrackAtHighTemperatureBand,
        growthLightSetting1,       growthLightSetting2,       growthLightSetting3,
        growthLightThresholdLow1,  growthLightThresholdLow2,  growthLightThresholdLow3,
        growthLightThresholdHigh1, growthLightThresholdHigh2, growthLightThresholdHigh3,
        screenCrack, screenPerfection;
private:
    double maxScreen;
};

} //namespace


#endif
