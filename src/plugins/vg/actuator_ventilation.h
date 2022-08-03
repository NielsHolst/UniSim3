/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
    double getFlux() const;
    void setFlux(double newFlux);
private:
    // Inputs
    double minFlux, maxFlux,
        windCoef, temperatureCoef,
        windSpeed, outdoorsTemperature, indoorsTemperature,
        effectiveVentArea, groundArea;
    // Output
    double flux, relative;
};
} //namespace


#endif
