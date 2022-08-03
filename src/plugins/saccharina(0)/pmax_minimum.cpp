/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "pmax_minimum.h"

using namespace base;

namespace saccharina {

PUBLISH(PmaxMinimum)

PmaxMinimum::PmaxMinimum(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates minimum Pmax from g");
    Input(g).imports("parameters/g[value]");
    Input(slope).equals(0.0533).help("Slope of Pmax on g");
    Input(intercept).equals(0.00016).help("Intercept of Pmax on g");
    Input(floor).help("Lower bound on Pmax");
    Output(value).help("Minimum Pmax");
}

void PmaxMinimum::reset() {
    update();
}

void PmaxMinimum::update() {
    value = g*slope + intercept;
    if (value < floor)
        value = floor;
}

}
