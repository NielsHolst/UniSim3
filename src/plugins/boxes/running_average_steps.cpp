/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "running_average_steps.h"

using namespace base;

namespace boxes {

PUBLISH(RunningAverageSteps)

RunningAverageSteps::RunningAverageSteps(QString name, Box *parent)
    : RunningSumSteps(name, parent)
{
    help("computes running average by no. of steps");
}

void RunningAverageSteps::update() {
    RunningSumSteps::update();
    value /= bufferSize();
}

} //namespace

