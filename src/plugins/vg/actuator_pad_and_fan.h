/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef ACTUATOR_PAD_AND_FAN_H
#define ACTUATOR_PAD_AND_FAN_H
#include <base/box.h>


namespace vg {

class ActuatorPadAndFan : public base::Box
{
public: 
    ActuatorPadAndFan(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    QString productName;
    int number;
    double groundArea, outdoorsTemperature, outdoorsRh,
        state;
    // Outputs
    double powerUse, cooling, vapourFlux;
    // Imported product inputs
    double
        coolingEfficiency, powerUseMax, flowRateMax, powerUserParasitic;
};

} //namespace
#endif
