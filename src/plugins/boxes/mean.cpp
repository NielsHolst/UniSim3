/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "mean.h"

using namespace base;
using vector_op::average;

namespace boxes {

PUBLISH(Mean)

Mean::Mean(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes average of inputs");
    Input(values).help("Input values");
    Output(value).help("Average of inputs; zero if no inputs");
    Output(count).help("Number of inputs");
}

void Mean::reset() {
    count = values.size();
    update();
}

void Mean::update() {
    value = average(values);
}

} //namespace

