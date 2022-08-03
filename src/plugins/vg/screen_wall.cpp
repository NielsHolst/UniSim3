/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "screen_wall.h"

using namespace base;

namespace vg {

PUBLISH(ScreenWall)


ScreenWall::ScreenWall(QString name, Box *parent)
    : Screen(name, parent)
{
    help("models a screen on wall side or end");
    Input(faceArea).imports("../../area[value]",CA);
}

double ScreenWall::computeMaxArea() {
    return faceArea;
}

} //namespace

