/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "trapezoid.h"

using namespace base;

namespace saccharina {

PUBLISH(Trapezoid)

Trapezoid::Trapezoid(QString name, QObject *parent)
    : Box(name, parent)
{
    help("models a trapezoid of three line segmentsT");
    Input(T).unit("oC").help("Current temperature");
    Input(T0).unit("oC").help("Minimum temperature");
    Input(Topt1).unit("oC").help("Lower temperature optimum");
    Input(Topt2).unit("oC").help("Higher temperature optimum");
    Input(Tmax).unit("oC").help("Maximum temperature");
    Output(value).unit("[0;1]").help("Scaling factor");
}

void Trapezoid::reset() {
   update();
}

void Trapezoid::update() {
    // Note: Inequalities rule out division by zero
    if (T < T0)
        value = 0.;
    else if (T < Topt1)
        value = (T-T0)/(Topt1-T0);
    else if (T < Topt2)
        value = 1.;
    else if (T < Tmax)
        value = (Tmax-T)/(Tmax-Topt2);
    else
        value = 0.;
}

}
