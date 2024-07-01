/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <boost/numeric/conversion/cast.hpp>
#include <base/publish.h>
#include <base/time_unit.h>
#include "running_sum_timed.h"

using namespace base;
using boost::numeric_cast;

namespace boxes {

PUBLISH(RunningSumTimed)

RunningSumTimed::RunningSumTimed(QString name, Box *parent)
    : RunningBase(name, parent)
{
    help("computes running sum in a time window");
    Input(timeWindow).equals(1).help("Time window to average over in units of timeUnit").unit("int>0");
    Input(timeUnit).equals("d").help("Unit of time window").unit("y|d|h|m|s");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Output(count).help("Number of steps averages over").unit("integer");
}

void RunningSumTimed::reset() {
    double timeWindowSecs = timeWindow * Time::toSeconds(timeUnit);
    count = numeric_cast<int>(std::floor(timeWindowSecs/timeStepSecs + 0.5));
    RunningBase::reset();
}

int RunningSumTimed::bufferSize() const {
    return count;
}

} //namespace

