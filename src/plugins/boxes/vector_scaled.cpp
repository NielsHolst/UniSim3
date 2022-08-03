/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "vector_scaled.h"

using namespace base;

namespace boxes {

PUBLISH(VectorScaled)

VectorScaled::VectorScaled(QString name, Box *parent)
    : Box(name, parent)
{
    help("scales a vector by a factor");
    Input(vector).help("Vector to be scaled");
    Input(factor).help("Scaling factor");
    Output(value).help("Scaled vector");
}

void VectorScaled::reset() {
    update();
}

void VectorScaled::update() {
    vector_op::product(value, vector, factor);
}

}
