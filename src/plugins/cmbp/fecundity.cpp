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
    help("models life time net per capita fecundity of P. truncatus");
    Input(a).equals(4.525).help("Parabola specifying female R0 (aT^2 + bT + c");
    Input(b).equals(-186.3).help("Parabola specifying female R0 (aT^2 + bT + c");
    Input(c).equals(1959.).help("Parabola specifying female R0 (aT^2 + bT + c");
    Input(T).help("Store temperature (oC)");
    Input(sexRatio).equals(0.5).help("Sex ratio [0;1]");
    Input(calibration).equals(1.).help("Relatoive calibration");
    Output(value).help("Life-time fecundity scaled by temperature");
}

void Fecundity::reset() {
    update();
}

void Fecundity::update() {
    if (T < 20.)
        value = f(20.);
    else if (T > 30.)
        value = f(30.);
    else
        value = f(T);
}

double Fecundity::f(double T) const {
    return calibration*sexRatio*(a*T*T + b*T + c);
}

}
