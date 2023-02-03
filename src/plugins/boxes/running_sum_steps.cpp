/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "running_sum_steps.h"

using namespace base;

namespace boxes {

PUBLISH(RunningSumSteps)

RunningSumSteps::RunningSumSteps(QString name, Box *parent)
    : RunningBase(name, parent)
{
    help("computes running sum by no. of steps");
    Input(count).equals(1).help("Number of steps to average over").unit(">0");
}

int RunningSumSteps::bufferSize() const {
    return count;
}

} //namespace

