/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ACTUATOR_HEAT_PIPE_H
#define ACTUATOR_HEAT_PIPE_H

#include <base/box.h>

namespace vg {

class ActuatorHeatPipe : public base::Box
{
public:
    ActuatorHeatPipe(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double volume, flowRate, minTemperature, maxTemperature,
        k, b, propLw,
        indoorsTemperature, groundArea, timeStep;
    // Outputs
    double transitTime,
        inflowTemperature, outflowTemperature, temperatureDrop,
        energyFlux,
        lwEmissionTop, lwEmissionBottom,
        convectionTop, convectionBottom;
    bool isHeating;
    // Data
    double _heatBuffer;
};

} //namespace


#endif
