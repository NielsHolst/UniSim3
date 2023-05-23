/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ACTUATOR_SCREEN_H
#define ACTUATOR_SCREEN_H
#include <base/box.h>

namespace vg {

class ActuatorScreen : public base::Box
{
public: 
    ActuatorScreen(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double lagPeriod, desiredState, timeStepSecs;
    // Outputs
    double state;
    // Data
    double _deltaStateMax;
};

} //namespace

#endif
