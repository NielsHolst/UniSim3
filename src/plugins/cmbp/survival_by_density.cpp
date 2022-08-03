/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** William Meikle, USDA [william.meikle at usda.gov].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "survival_by_density.h"

using namespace base;

namespace cmbp {

PUBLISH(SurvivalByDensity)

SurvivalByDensity::SurvivalByDensity(QString name, Box *parent)
    : Box(name, parent)
{
    Input(e1).help("Regression parameter");
    Input(N).help("Adult density");
    Output(value).help("Peak curve");
}

void SurvivalByDensity::reset() {
    update();
}

void SurvivalByDensity::update() {
    value = exp(e1*sqrt(N));
}


}
