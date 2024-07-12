/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
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
    QString productName;
    int number;
    double groundArea, indoorsTemperature, indoorsRh, state;
    bool sendToBuffer;
    // Outputs
    double
        powerUseCooling,
        powerUserParasitic,
        powerUse,
        cooling,
        energyToBuffer,
        condensation,
        maxCoolingLoad;
    // Data (copied from heat pump product)
    struct {
        double maxCoolingLoad, coolingEfficiency, maxFlowRate, parasiticLoad, coolingTemperature;
    } p;

};

} //namespace
#endif
