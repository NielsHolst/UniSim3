/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include "setpoints.h"

using namespace base;

namespace vg {

PUBLISH(Setpoints)

#define ELEMENT(x) Input(x).imports("elementary/" #x "[value]", CA)
#define SCREEN(x)  Input(x).imports("elementary/screens/" #x "[value]", CA)
#define LIGHT(x,v,i) Input(x##i).imports("elementary/growthLights/growthLight" #i "/" #v "[value]", CA)

Setpoints::Setpoints(QString name, Box *parent)
    : Box(name, parent)
{
    help("holds all setpoints");
    ELEMENT(heatingTemperatureNormalRh).unit("oC").help("Normal temperature for heating");
    ELEMENT(heatingTemperatureHighRhMargin).unit("oC").help("Margin for increasing heating setpoint at high RH (above rhMax)");
    ELEMENT(heatingTemperatureLowRhMargin).unit("oC").help("Margin for increasing heating setpoint at low RH (below rhMin)");
    ELEMENT(rhMax).unit("[0;100]").help("RH is too high above this setpoint");
    ELEMENT(rhMaxBand).unit("[0;100]").help("Proportional band for increasing response over rhMax");
    ELEMENT(rhMin).unit("[0;100]").help("RH is too low below rhMin+rhMinBand");
    ELEMENT(rhMinBand).unit("[0;100]").help("Proportional band for increasing response below rhMin+rhMinBand");
    ELEMENT(ventilationTemperatureMargin).unit("oC").help("Fixed margin above heating setpoint for ventilation");
    ELEMENT(crackVentilationNormalTemperature).unit("[0;1]").help("Max. vent crack opening at high RH (RH>=rhMax+rhMaxBand)");
    ELEMENT(crackVentilationTemperatureMin).unit("oC").help("Crack closed below this temparature");
    ELEMENT(crackVentilationTemperatureMinBand).unit("oC").help("Crack start closing at low temperature (T<crackVentTempMin+crackVentTempMinBand)");
    ELEMENT(screenCrackAtHighRh).unit("[0;1]").help("Max. screen crack opening at high RH (RH>=rhMax+rhMaxBand)");
    ELEMENT(screenCrackAtHighTemperature).unit("[0;1]").help("Max. screen crack opening at high T (T>=ventTemp+screenCrackAtHighTempBand)");
    ELEMENT(screenCrackAtHighTemperatureBand).unit("oC").help("Crack starts opening at high T (T>ventTemp)");
    ELEMENT(co2Capacity).unit("g/m2/h").help("Max. CO2 injection rate");
    ELEMENT(co2Setpoint).unit("ppm").help("Desired CO2 concentration");
    ELEMENT(co2VentilationThreshold).unit("[0;1]").help("CO2 injection rate falls below capacity at ventilation above this threshold");
    ELEMENT(co2VentilationBand).unit("[0;1]").help("CO2 injection rate falls to zero above this band");
    ELEMENT(chalk).unit("").help("");
    SCREEN(screenEnergyThreshold1);
    SCREEN(screenEnergyThreshold2);
    SCREEN(screenEnergyThresholdBand);
    SCREEN(screenShadeThreshold1);
    SCREEN(screenShadeThreshold2);
    SCREEN(screenShadeThresholdBand);
    SCREEN(screenFixed1);
    SCREEN(screenFixed2);
    LIGHT(growthLightSetting, setting, 1);
    LIGHT(growthLightSetting, setting, 2);
    LIGHT(growthLightSetting, setting, 3);
    LIGHT(growthLightThresholdLow,  thresholdLow,  1);
    LIGHT(growthLightThresholdLow,  thresholdLow,  2);
    LIGHT(growthLightThresholdLow,  thresholdLow,  3);
    LIGHT(growthLightThresholdHigh, thresholdHigh, 1);
    LIGHT(growthLightThresholdHigh, thresholdHigh, 2);
    LIGHT(growthLightThresholdHigh, thresholdHigh, 3);
    Input(screenCrack).imports("./screenCrack[value]",CA).unit("[0;1]").help("Screen crack held open");
    Input(screenPerfection).imports("shelter[screenPerfection]");
    Output(maxScreen).unit("[0;1]").help("Max. setting of screen");
}

void Setpoints::amend() {
    // Assume that if avgRh exists, so do the remaining children
    if (findMaybeOne<Box*>("./avgRh"))
        return;
    // Build child boxes
    BoxBuilder builder(this);
    builder.
    box("RunningAverageTimed").name("avgRh").
        port("initial").imports("indoors/humidity[rh]",CA).
        port("input").imports("indoors/humidity[rh]",CA).
        port("timeWindow").equals(20.).
        port("timeUnit").equals("m").
    endbox().
    box("ProportionalSignal").name("heatingTemperatureHighRhIncrement").
        port("input").imports("../avgRh[value]",CA).
        port("threshold").imports("..[rhMax]",CA).
        port("thresholdBand").imports("..[rhMaxBand]",CA).
        port("increasingSignal").equals(true).
        port("minSignal").equals(0.).
        port("maxSignal").imports("..[heatingTemperatureHighRhMargin]",CA).
    endbox().
    box("ProportionalSignal").name("heatingTemperatureLowRhIncrement").
        port("input").imports("../avgRh[value]",CA).
        port("threshold").imports("..[rhMin]",CA).
        port("thresholdBand").imports("..[rhMinBand]",CA).
        port("increasingSignal").equals(false).
        port("minSignal").equals(0.).
        port("maxSignal").imports("..[heatingTemperatureLowRhMargin]",CA).
    endbox().
    box("SetpointHeating").name("heatingTemperature").
    endbox().
    box("SetpointVentilation").name("ventilationTemperature").
    endbox().
    box("ProportionalSignal").name("crackVentilation").
        port("input").imports("indoors/humidity[rh]",CA).
        port("threshold").imports("setpoints[rhMax]",CA).
        port("thresholdBand").imports("setpoints[rhMaxBand]",CA).
        port("increasingSignal").equals(true).
        port("maxSignal").imports("./maxCrackDepOnTemperature[value]").
        port("minSignal").equals(0).
        box("ProportionalSignal").name("maxCrackDepOnTemperature").
            port("input").imports("outdoors[temperature]",CA).
            port("threshold").imports("../..[crackVentilationTemperatureMin]",CA).
            port("thresholdBand").imports("../..[crackVentilationTemperatureMinBand]",CA).
            port("increasingSignal").equals(true).
            port("maxSignal").imports("../..[crackVentilationNormalTemperature]",CA).
            port("minSignal").equals(0).
        endbox().
    endbox().
    box("Maximum").name("screenCrack").
        port("values").imports("./*[value]")   .
        box("ProportionalSignal").name("atHighRh").
            port("input").imports("indoors/humidity[rh]",CA).
            port("threshold").imports("../..[rhMax]",CA).
            port("thresholdBand").imports("../..[rhMaxBand]",CA).
            port("increasingSignal").equals(true).
            port("minSignal").equals(0.).
            port("maxSignal").imports("../..[screenCrackAtHighRh]",CA).
        endbox().
        box("ProportionalSignal").name("atHighTemperature").
            port("input").imports("indoors/temperature[value]",CA).
            port("threshold").imports("../../ventilationTemperature[value]",CA).
            port("thresholdBand").imports("../..[screenCrackAtHighTemperatureBand]",CA).
            port("increasingSignal").equals(true).
            port("minSignal").equals(0.).
            port("maxSignal").imports("../..[screenCrackAtHighTemperature]",CA).
        endbox().
    endbox();
}

void Setpoints::reset() {
    update();
}

void Setpoints::update() {
    maxScreen = 1. - screenCrack;
}

} //namespace

