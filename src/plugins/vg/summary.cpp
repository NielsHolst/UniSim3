/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "summary.h"

using namespace base;

namespace vg {
	
PUBLISH(Summary)

Summary::Summary(QString name, Box *parent)
	: Box(name, parent)
{
    help("summarises greenhouse budget");
    Input(heatPipesHeatFlux).imports("actuators/heatPipes[heatFlux]").unit("W/m2");
    Input(lampsPowerUsage).imports("actuators/growthLights[powerUsage]").unit("W/m2");
    Input(co2Injection).imports("actuators/co2[value]").unit("g/m2/h");
    Input(transpirationFinite).imports("budget[transpiration]");
    Input(condensationFinite).imports("budget[condensation]");
    Input(ventedWaterFinite).imports("budget[ventedWater]");
    Input(screenState1).imports("actuators/screens/layer1[state]");
    Input(screenState2).imports("actuators/screens/layer2[state]");
    Input(screenState3).imports("actuators/screens/layer3[state]");
    Input(isSkipping).imports("output/*[isSkipping]");
    Input(lightState1).imports("actuators/growthLights/bank1[isOn]");
    Input(lightState2).imports("actuators/growthLights/bank2[isOn]");
    Input(lightState3).imports("actuators/growthLights/bank3[isOn]");
//    Input(timeStep).imports("calendar[timeStepSecs]");
    Output(heatingCost).unit("MWh/m2/y");
    Output(lampCost).unit("MWh/m2/y");
    Output(co2Cost).unit("kg CO2/m2/y");
    Output(transpiration).unit("kg/m2/y");
    Output(condensation).unit("kg/m2/y");
    Output(ventedWater).unit("kg/m2/y");
    Output(screen1).unit("[0;1]").help("Proportion of time screen layer 1 was drawn");
    Output(screen2).unit("[0;1]").help("Proportion of time screen layer 2 was drawn");
    Output(screen3).unit("[0;1]").help("Proportion of time screen layer 3 was drawn");
    Output(light1).unit("[0;1]").help("Proportion of time growth lights bank 1 was on");
    Output(light2).unit("[0;1]").help("Proportion of time growth lights bank 2 was on");
    Output(light3).unit("[0;1]").help("Proportion of time growth lights bank 3 was on");
}

void Summary::reset() {
    _heatingSum = _lampSum = _co2Sum =
        _screenState1Sum =
        _screenState2Sum =
        _screenState3Sum =
        _lightState1Sum  =
        _lightState2Sum  =
        _lightState3Sum  = 0.;
    _n = 0;
}

void Summary::update() {
    if (!isSkipping) {
        // Update sums
        ++_n;
        _heatingSum      += heatPipesHeatFlux;
        _lampSum         += lampsPowerUsage;
        _co2Sum          += co2Injection;
        transpiration    += transpirationFinite;
        condensation     += condensationFinite;
        ventedWater      += ventedWaterFinite;
        _screenState1Sum += screenState1;
        _screenState2Sum += screenState2;
        _screenState3Sum += screenState3;
        _lightState1Sum  += lightState1;
        _lightState2Sum  += lightState2;
        _lightState3Sum  += lightState3;
        // Update costs
        heatingCost     = _heatingSum/_n*24*365*1e-6;
        lampCost        = _lampSum/_n   *24*365*1e-6;
        co2Cost         = _co2Sum/_n    *24*365*1e-3;
        // Update actuators
        screen1 = _screenState1Sum/_n;
        screen2 = _screenState2Sum/_n;
        screen3 = _screenState3Sum/_n;
        light1  = _lightState1Sum/_n;
        light2  = _lightState2Sum/_n;
        light3  = _lightState3Sum/_n;
    }
}

} //namespace

