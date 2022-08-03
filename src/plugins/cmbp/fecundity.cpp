/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** William Meikle, USDA [william.meikle at usda.gov].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "fecundity.h"

using namespace base;

namespace cmbp {

PUBLISH(Fecundity)

Fecundity::Fecundity(QString name, Box *parent)
    : Box(name, parent)
{
    Input(maxFecundity).help("Life-time fecundity of a female at 30oC");
    Input(r).help("Increase in fecundity with temperature");
    Input(T).help("Store temperature (oC)");
    Input(sexRatio).equals(0.5).help("Sex ratio [0;1]");
    Output(value).help("Life-time fecundity scaled by temperature");
}

void Fecundity::reset() {
    update();
}

void Fecundity::update() {
    value = (T>30.) ? maxFecundity : exp(r*T)/exp(r*30)*maxFecundity;
    value *= sexRatio;
}


}
