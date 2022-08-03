/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** William Meikle, USDA [william.meikle at usda.gov].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "dev_rate.h"

using namespace base;

namespace cmbp {

PUBLISH(DevRate)

DevRate::DevRate(QString name, Box *parent)
    : Box(name, parent)
{
    Input(b1).help("Regression parameter");
    Input(b2).help("Regression parameter");
    Input(b3).help("Regression parameter");
    Input(b4).help("Regression parameter");
    Input(T).help("Ambient temperature (oC)");
    Output(value).help("Daily development rate");
}

void DevRate::reset() {
    update();
}

void DevRate::update() {
    value = exp(b1*T) - exp(b1*b2 - (b2-T)/b3) + b4;
}


}
