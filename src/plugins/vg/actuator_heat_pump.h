/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ACTUATOR_HEAT_PUMP_H
#define ACTUATOR_HEAT_PUMP_H
#include <base/box.h>


namespace vg {

class ActuatorHeatPump : public base::Box
{
public: 
    ActuatorHeatPump(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double groundArea, indoorsTemperature, indoorsRh,
        maxCoolingPower, coolingEfficiency, maxFlowRate, maxPowerUserParasitic, coolingTemperature,
        state;
    int number;
    // Outputs
    double
        condensationRate,
        powerUseCooling,
        powerUserParasitic,
        powerUse,
        heat;
};

} //namespace
#endif
