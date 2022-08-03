/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
    Input(lookAhead).equals(0.).help("Computed error from predicted sensed value this time ahead").unit("min");

    Input(minimum).equals(doubleMin).help("Minimum allowed value of control variable");
    Input(maximum).equals(doubleMax).help("Maximum allowed value of control variable");
    Input(timeStep).imports("calendar[timeStepSecs]").unit("s");

    Output(controlVariable).help("The control variable; tends to zero when all three error terms summed tend to zero");
    Output(error).help("The error");
    Output(integral).help("The integral error").unit("min");
    Output(derivative).help("The derivative of the approach of sensed towards target value").unit("/min");
    Output(eta).help("Estimated time of arrival at desired value; negative means wrong direction").unit("min");
}

void PidController::reset() {
    dt = timeStep/60.;  // from secs to minutes
}

void PidController::update() {
    updateControlVariable();
    prevSensedValue = sensedValue;
}

void PidController::updateControlVariable() {
    // Compute errors
    derivative = (error == 0. || tick++ < 2) ? 0. : (sensedValue-prevSensedValue)/dt;
    error = desiredValue - (sensedValue + derivative*lookAhead);
    integral += error*dt;
    // Compute control response
    controlVariable = Kprop*error + Kint*integral + Kderiv*derivative*dt;
    controlVariable = qBound(minimum, controlVariable, maximum);
    // Compute estimated time of arrival
    eta = (derivative == 0.) ? 0. : error/derivative;;
}

} //namespace

