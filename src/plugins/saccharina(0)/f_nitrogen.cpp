/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/dialog.h>
#include <base/publish.h>
#include "f_nitrogen.h"

using namespace base;
using phys_math::sqr;

namespace saccharina {

PUBLISH(Fnitrogen)

Fnitrogen::Fnitrogen(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the effect of nitrogen reserves on growth rate");
    Input(proportion).imports("reserves/nitrogen[proportion]");
    Input(minValue).equals(0.545).help("Minimum scaling value");
    Input(slope).equals(7.85).help("Slope of sigmoid curve");
    Input(n50).equals(0.00756).help("Nitrogen reserves proportion with 50% effect");
    Output(value).unit("[0;1]").help("Scaling of growth rate");
}

void Fnitrogen::reset() {
   update();
}

void Fnitrogen::update() {
    value = minValue + (1.-minValue)/(1 + exp(-slope*(log(proportion) - log(n50))));
}

}
