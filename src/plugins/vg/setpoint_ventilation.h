/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SETPOINT_VENTILATION_H
#define SETPOINT_VENTILATION_H
#include <base/box.h>

namespace vg {

class SetpointVentilation : public base::Box
{
public:
    SetpointVentilation(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double setpointHeating, ventilationTemperatureMargin;
    // Outputs
    double value;
};

} //namespace


#endif
