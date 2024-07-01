/* Copyright 1998-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** William Meikle, USDA [william.meikle at usda.gov].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "exponential_peak.h"

using namespace base;

namespace cmbp {

PUBLISH(ExponentialPeak)

ExponentialPeak::ExponentialPeak(QString name, Box *parent)
    : Box(name, parent)
{
    Input(d1).help("Regression parameter");
    Input(d2).help("Regression parameter");
    Input(x).help("Independent variable");
    Input(optimum).help("x-value at peak y");
    Output(value).help("Peak curve");
}

void ExponentialPeak::reset() {
    update();
}

void ExponentialPeak::update() {
    value = exp(-0.5*pow( fabs(x-optimum)/d1, d2));
}


}
