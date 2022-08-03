/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "heat_exchanger.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(HeatExchanger)

HeatExchanger::HeatExchanger(QString name, Box *parent)
    : Box(name, parent)
{
    help("models a heat exchanger");
    Input(groundArea).imports("construction/geometry[groundArea]",CA);
    Input(outdoorsTemperature).imports("outdoors/temperature[value]",CA);
    Input(indoorsTemperature).imports("indoors/temperature[value]",CA);
    Input(indoorsRh).imports("indoors/humidity[rh]",CA);
    Input(number).equals(1).help("Number of heat pumps").unit("0,1,2,...");
    Input(efficiency).equals(0.69).help("Efficiency of exchanging temperature difference indoors/outdoors").unit("[0;1]");
    Input(maxFlowRate).equals(2.5).help("Maximum air flow rate").unit("m3/s");
    Input(maxPowerUserHeatExchange).equals(5).help("Maximum power use for heat exchange").unit("kW");
    Input(maxPowerUserParasitic).equals(2.2).help("Maximum basic power use").unit("kW");
    Input(state).equals(1.).help("Running state relative to full effect").unit("[0;1]");

    Output(condensationRate).help("Rate of water condensed in the unit").unit("kg/m2/s");
    Output(powerUseCooling).help("Power used for cooling").unit("kW/m2");
    Output(powerUserParasitic).help("Power lost").unit("kW/m2");
    Output(powerUse).help("Power used (total)").unit("kW/m2");
}

void HeatExchanger::reset() {
    update();
}

void HeatExchanger::update() {
    // Compute for one unit
    double
        deltaT = std::max(indoorsTemperature - outdoorsTemperature, 0.)*efficiency,
        outputT = indoorsTemperature - deltaT,
        indoorsAh = ahFromRh(indoorsTemperature, indoorsRh), // kg/m3
        outputAh = ahFromRh(outputT, 100.); // kg/m3
    condensationRate = std::max(indoorsAh - outputAh, 0.)*maxFlowRate*state;   // kg/s
    powerUseCooling = maxPowerUserHeatExchange*state;
    powerUserParasitic = maxPowerUserParasitic*state;
    powerUse =  powerUseCooling + powerUserParasitic;
    // Correct for number of units and area
    double k = number/groundArea;
    condensationRate *= k;
    powerUseCooling *= k;
    powerUserParasitic *= k;
    powerUse *= k;
}

} //namespace

