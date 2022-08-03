/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "rain_interception.h"

using namespace base;
using namespace std;

namespace coffee {

PUBLISH(RainInterception)

RainInterception::RainInterception(QString name, QObject *parent)
    : Box(name, parent) {
    help("computes rain intercepted by coffee and trees");
    Input(interceptionCoef).equals(0.25).unit("mm/m2/m2/d").help("Interception depending on leaf area index");
    Input(rainfall).imports("weather[Rain]");
    Input(laiCoffee).imports("/*/coffee[lai]");
    Input(laiTree).imports("tree[lai]");
    Input(timeStep).imports("calendar[timeStepDays]");
    Output(coffee).unit("mm").help("Rain intercepted by coffee");
    Output(tree).unit("mm").help("Rain intercepted by trees");
    Output(total).unit("mm").help("Rain intercepted in total");
}

void RainInterception::update() {
    tree   = min(rainfall, interceptionCoef*laiTree)*timeStep;
    coffee = min(rainfall - tree, interceptionCoef*laiCoffee)*timeStep;
    total  = tree + coffee;
}

} //namespace

