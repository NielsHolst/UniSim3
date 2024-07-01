/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <base/box.h>

namespace vg {

class PidController : public base::Box
{
public:
    PidController(QString name, Box *parent);
    void reset();
    void update();
//    void rollBack();
private:
    // Inputs
    double sensedValue, desiredValue,
        minimum, maximum,
        timeStep,
        Kprop, Kint, Kderiv;
    // Outputs
    double controlVariable, error, integral, derivative,
    // Data
        _prevError;
//    struct {
//        double controlVariable, error, integral, derivative,
//           prevError;

//    } latest;
};
} //namespace


#endif
