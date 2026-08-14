/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "bph_survival.h"

using namespace base;

namespace bph {

PUBLISH(BphSurvival)

BphSurvival::BphSurvival(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes BPH density-dependenty survival");
    Input(density).help("BPH density");
    Input(atDensity).equals(1).help("At this density use set survival");
    Input(survival).help("Set survival");
    Output(value).help("BPH survival");
}

void BphSurvival::reset() {
//    _slope = -log(survival)/atDensity;
    value = 1;
}

void BphSurvival::update() {
//    value = exp(-_slope*density);
    value = 1. - density/atDensity;
    if (value<=0.) value = 1e-9;
}


}
