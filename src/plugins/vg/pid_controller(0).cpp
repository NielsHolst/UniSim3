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
    Input(controlledValue).imports(".[controlVariable]").help("Value used for derivate control (Kderiv)");
    Input(desire).equals("KeepAt").unit("KeepAt|KeepBelow|KeepAbove").help("Keep sensed value at, below or above desired value");
    Input(Kprop).equals(0.1).help("The proportional gain").unit("/min");
    Input(Kint).equals(0.).help("The integral gain").unit("/min");
    Input(Kderiv).equals(0.).help("The derivative gain").unit("/min");

    Input(minimum).equals(doubleMin).help("Minimum allowed value of control variable");
    Input(maximum).equals(doubleMax).help("Maximum allowed value of control variable");
    Input(timeStep).imports("calendar[timeStepSecs]").unit("s");

    Output(controlVariable).help("The control variable; tends to zero when all three error terms summed tend to zero");
    Output(error).help("The error");
    Output(errorIntegral).help("The integral error");
    Output(errorDerivative).help("The derivative error");
}

void PidController::reset() {
    prevControlledValue = prevError = 0.;
    dt = timeStep/60.;  // from secs to minutes
    tick = 0;
    QString s = desire.toLower();
    if (s=="keepat")
        _desire = KeepAt;
    else if (s=="keepbelow")
        _desire = KeepBelow;
    else if (s=="keepabove")
        _desire = KeepAbove;
    else
        ThrowException("Desire must be one of KeepAt|KeepBelow|KeepAbove").value(desire).context(this);
}

void PidController::update() {
    updateControlVariable();
    prevControlledValue = controlledValue;
    prevError = error;
}

void PidController::updateControlVariable() {
    switch (_desire) {
    case KeepBelow:
        error = max(sensedValue - desiredValue, 0.);
        break;
    case KeepAbove:
        error = max(desiredValue - sensedValue, 0.);
        break;
    case KeepAt:
        error = desiredValue - sensedValue;
        errorIntegral += error;
        break;
    }
    errorDerivative = (error == 0. || tick++ < 2) ? 0. : (controlledValue - prevControlledValue)/dt;
    controlVariable = (Kprop*error + Kint*errorIntegral + Kderiv*errorDerivative)*dt;
    controlVariable = qBound(minimum, controlVariable, maximum);
}

} //namespace

