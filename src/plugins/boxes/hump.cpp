/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "hump.h"

using namespace base;

namespace boxes {

PUBLISH(Hump)

Hump::Hump(QString name, Box *parent)
    : Box(name, parent)
{
    help("produces a hump- or ditch-shaped curve");
    Input(x).help("x-value at which to calculate curve value");
    Input(x0).equals(0).help("Beginning of curve");
    Input(x1).equals(1).help("End of curve");;
    Input(ymin).equals(0).help("Minimum value of curve");
    Input(ymax).equals(1).help("Maximum value of curve");;
    Input(ditch).equals(false).help("Is the curve a ditch?");
    Output(value).help("Curve value at x-value");
}

void Hump::initialize() {
    if (x0>=x1) ThrowException("x0 < x1 must be true").
            value(x0).value2(x1).context(this);
    if (ymin>=ymax) ThrowException("ymin < ymax must be true").
            value(ymin).value2(ymax).context(this);
}

void Hump::reset() {
    update();
}

void Hump::update() {
    // Compute y as a parabolic curve [0;1]
    double y = (x<x0 || x>x1) ?
                0. : 4.*(x-x0)*(x-x1)/(x0-x1)/(x1-x0);
    // Reshape and rescale y
    value = y*y*(ymax - ymin);
    value = ditch ? ymax - value : value + ymin;
}

} //namespace

