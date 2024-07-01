/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef ACTUATOR_VENTILATION_H
#define ACTUATOR_VENTILATION_H

#include <base/box.h>

namespace vg {

class ActuatorVentilation : public base::Box
{
public:
    ActuatorVentilation(QString name, Box *parent);
    void reset();
    void update();
    void setOpening(double newOpening);
private:
    // Inputs
    double
        windCoef, temperatureCoef,
        leakage, crackOpening, opening,
        windSpeed,
        outdoorsTemperature, indoorsTemperature,
        outdoorsRh, indoorsRh,
        indoorsVolume, ventAreaRatio, timeStep;
    // Output
    double
        maxVentFlux, ventFlux,
        value;
    bool isVentilating;
    // Methods
    void updateOutput();
};
} //namespace


#endif
