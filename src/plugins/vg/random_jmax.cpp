/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "random_jmax.h"

using namespace base;

namespace vg {

PUBLISH(RandomJmax)

RandomJmax::RandomJmax(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes random Jmax from Vcmax");
    Input(intercept).equals(4.25).help("Intercept of Jmax-Vcmax relation");
    Input(slope).equals(0.55).help("Slope of Jmax-Vcmax relation");
    Input(Vcmax).help("Vcmax value");
    Input(deviation).help("Relative deviattion from regression line");
    Output(value).help("Random Jmax value");
}

void RandomJmax::reset() {
    update();
}

void RandomJmax::update() {
    value = (intercept + slope*Vcmax)*(1. + deviation);
}

} //namespace

