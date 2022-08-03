/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "maximum.h"

#include <base/dialog.h>

using namespace base;
using vector_op::max;

namespace boxes {

PUBLISH(Maximum)

Maximum::Maximum(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes maximum of inputs");
    Input(values).help("Input values");
    Output(value).help("Maximum of inputs; zero if no inputs");
    Output(count).help("Number of inputs");
}

void Maximum::reset() {
    count = values.size();
    update();
}

void Maximum::update() {
    value = max(values);
}

} //namespace

