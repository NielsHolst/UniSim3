/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Isaac Kwesi Abuley , Aarhus University [ikabuley at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "potato_development_at_temp.h"
using namespace base;

namespace potato {

PUBLISH(PotatoDevelopmentAtTemp)

PotatoDevelopmentAtTemp::PotatoDevelopmentAtTemp(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(temp).help("Temperature used to calculate development");
    Input(Tfloor).equals(7.).help("The base temperature threshold, if not specified then the default is 7C");
    Input(Tceiling).equals(30.).help("The maximum temperature threshold, if not sepecified then the default is 30C");
    Input(Topt).equals(21.).help("The optimum temperature threshold, if not specified then the defualt is 21C");
    Output(step).help("The Physiologic Development at this temperature in a day");
}

void PotatoDevelopmentAtTemp::reset() {
   update();
}

inline double sqr(double x) { return x*x; }

void PotatoDevelopmentAtTemp::update() {
    if (temp < Tfloor)
        step = 0.;
    else if (temp < Topt)
        step = 10. * (1. - sqr(temp - Topt)/sqr(Topt - Tfloor));
    else if (temp < Tceiling)
        step = 10. * (1. - sqr(temp - Topt)/sqr(Tceiling - Topt));
    else
        step = 0.;
}

}
