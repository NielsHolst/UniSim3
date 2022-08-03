/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ACTUATOR_HEAT_EXCHANGER_H
#define ACTUATOR_HEAT_EXCHANGER_H
#include <base/box.h>


namespace vg {

class ActuatorHeatExchanger : public base::Box
{
public: 
    ActuatorHeatExchanger(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double groundArea, outdoorsTemperature, indoorsTemperature, indoorsRh,
        efficiency, maxFlowRate, maxPowerUserHeatExchange, maxPowerUserParasitic,
        state;
    int number;
    // Outputs
    double
        condensationRate,
        powerUseCooling,
        powerUse,
        powerUserParasitic,
        powerUseheat;
};

} //namespace
#endif
