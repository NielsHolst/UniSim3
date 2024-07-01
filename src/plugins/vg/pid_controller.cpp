/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include "pid_controller.h"
#include <base/phys_math.h>
#include <base/publish.h>

#include <base/dialog.h>

using std::min;
using std::max;
using namespace base;
using namespace phys_math;

namespace vg {
	
PUBLISH(PidController)

const double doubleMin = std::numeric_limits<double>::lowest();
const double doubleMax = std::numeric_limits<double>::max();

PidController::PidController(QString name, Box *parent)
    : Box(name, parent)
{
    help("delivers PID control of a control variable");
    Input(sensedValue).help("The sensed value");
    Input(desiredValue).help("The desired value (setpoint)");
    Input(Kprop).equals(0.1).help("The proportional gain");
    Input(Kint).equals(0.).help("The integral gain").unit("/min");
    Input(Kderiv).equals(0.).help("The derivative gain").unit("min");

    Input(minimum).equals(doubleMin).help("Minimum allowed value of control variable");
    Input(maximum).equals(doubleMax).help("Maximum allowed value of control variable");
    Input(timeStep).imports("calendar[timeStepSecs]").unit("s");

    Output(controlVariable).help("The control variable; tends to zero when all three error terms summed tend to zero");
    Output(error).help("The error");
    Output(integral).help("The integral error");
    Output(derivative).help("The derivative of the approach of sensed towards target value").unit("/s");
}

void PidController::reset() {
    controlVariable =
    error =
    integral =
    derivative =
    _prevError = 0.;
}

void PidController::update() {
    // Compute errors
    error = desiredValue - sensedValue;
    integral += error*timeStep;
    derivative = (error - _prevError)/timeStep;
    _prevError = error;
    // Compute control response
    controlVariable = Kprop*error + Kint*integral + Kderiv*derivative;
    controlVariable = qBound(minimum, controlVariable, maximum);
}

} //namespace

