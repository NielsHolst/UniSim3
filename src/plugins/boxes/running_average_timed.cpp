/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <boost/numeric/conversion/cast.hpp>
#include <base/publish.h>
#include <base/time_unit.h>
#include <base/vector_op.h>
#include "running_average_timed.h"

using namespace base;
using boost::numeric_cast;
using vector_op::max;

namespace boxes {

PUBLISH(RunningAverageTimed)

RunningAverageTimed::RunningAverageTimed(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes running average in time window");
    Input(initial).help("Value of running average at reset");
    Input(input).help("The input to average");
    Input(timeWindow).equals(1).help("Time window to average over in units of timeUnit").unit("int>0");
    Input(timeUnit).equals("d").help("Unit of time step").unit("y|d|h|m|s");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Output(value).help("Running average");
    Output(count).help("Number of steps averages over").unit("integer");
}

void RunningAverageTimed::reset() {
    double timeWindowSecs = timeWindow * Time::toSeconds(timeUnit);
    count = numeric_cast<int>(std::floor(timeWindowSecs/timeStepSecs + 0.5));

    if (count<1) count = 1;
    buffer.fill(initial, count);
    value = initial;
    sum = initial*count;
    pos = 0;
}

void RunningAverageTimed::update() {
    sum += input - buffer.at(pos);
    buffer[pos] = input;
    pos = (pos+1)%count;
    value = sum/count;
}

} //namespace

