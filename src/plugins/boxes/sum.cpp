/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "sum.h"

using namespace base;
using vector_op::sum;

namespace boxes {

PUBLISH(Sum)

Sum::Sum(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes sum of inputs");
    Input(values).help("Input values");
    Output(value).help("Sum of inputs; zero if no inputs");
    Output(count).help("Number of inputs");
}

void Sum::reset() {
    count = values.size();
    update();
}

void Sum::update() {
    value = sum(values);
}

} //namespace

