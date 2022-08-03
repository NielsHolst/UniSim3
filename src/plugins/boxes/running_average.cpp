/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "running_average.h"

using namespace base;
using vector_op::max;

namespace boxes {

PUBLISH(RunningAverage)

RunningAverage::RunningAverage(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes running average by no. of steps");
    Input(initial).help("Value of running average at reset");
    Input(input).help("The input to average");
    Input(count).equals(1).help("Number of steps to average over").unit(">0");
    Output(value).help("Running average");
}

void RunningAverage::reset() {
    if (count<1) count = 1;
    buffer.fill(initial, count);
    value = initial;
    sum = initial*count;
    pos = 0;
}

void RunningAverage::update() {
    sum += input - buffer.at(pos);
    buffer[pos] = input;
    pos = (pos+1)%count;
    value = sum/count;
}

} //namespace

