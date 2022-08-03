/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SETPOINT_HEATING_H
#define SETPOINT_HEATING_H
#include <base/box.h>

namespace vg {

class SetpointHeating : public base::Box
{
public:
    SetpointHeating(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double
        heatingTemperatureNormalRh,
        heatingTemperatureHighRhIncrement,
        heatingTemperatureLowRhIncrement;
    // Outputs
    double value;
};

} //namespace


#endif
