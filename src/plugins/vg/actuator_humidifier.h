/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef ACTUATOR_HUMIDIFIER_H
#define ACTUATOR_HUMIDIFIER_H
#include <base/box.h>


namespace vg {

class ActuatorHumidifier : public base::Box
{
public: 
    ActuatorHumidifier(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    QString productName;
    int numberInstalled;
    double state, groundArea, greenhouseVolume, inflowTemperature, inflowRh, timeStep;
    // Outputs
    double efficiency, maxRate, parasiticLoad,
        powerUse, vapourFlux;
};

} //namespace
#endif
