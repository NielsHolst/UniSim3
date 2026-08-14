/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "transition.h"

using namespace base;

namespace eq {

PUBLISH(Transition)

Transition::Transition(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes a value between min-max limits based on an input and threshold");
    Input(input).help("The input that relates to the threshold");
    Input(threshold).equals(0.).help("The input threshold at which the output value begins changing");
    Input(thresholdBand).equals(1.).help("The interval above the threshold during which the output value is changing");;
    Input(minValue).equals(0.).help("The minimum value attained outside threshold band");
    Input(maxValue).equals(1.).help("The maximum value attained outside threshold band");
    Input(resetValue).equals(0.).help("The value at reset");
    Input(increasingValue).equals(true).help("Tells whether the output value is increasing inside the threshold band");
    Output(value).help("The output value");
}

void Transition::reset() {
    value = resetValue;
}

void Transition::update() {
    double x0{threshold},
           x1{threshold + thresholdBand},
           y0{increasingValue ? minValue : maxValue},
           y1{increasingValue ? maxValue : minValue};
    if (input <= x0)
        value = y0;
    else if (input >= x1)
        value = y1;
    else {
        if (thresholdBand <= 0.)
            ThrowException("thresholdBand must be positive").value(thresholdBand).context(this);
        double gap = input - x0;
        value = y0 + gap/thresholdBand*(y1 - y0);
    }
}

} //namespace

