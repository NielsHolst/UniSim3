/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "screen_roof.h"

using namespace base;

namespace vg {

PUBLISH(ScreenRoof)

ScreenRoof::ScreenRoof(QString name, Box *parent)
    : Screen(name, parent)
{
    help("models a screen towards the roof");
    Input(groundArea).imports("construction/geometry[groundArea]",CA);
}

double ScreenRoof::computeMaxArea() {
    // A roof screen covers half of the ground area;
    // we assume a horizontal roof screen
    return groundArea/2.;
}

} //namespace

