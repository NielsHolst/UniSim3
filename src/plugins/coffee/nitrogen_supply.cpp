/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "nitrogen_supply.h"

using namespace base;
using namespace std;

namespace coffee {

PUBLISH(NitrogenSupply)

NitrogenSupply::NitrogenSupply(QString name, QObject *parent)
    : Box(name, parent) {
    help("uses Michaelis-Menten model for nitrogen uptake");
    Input(efficiency).equals(0.002).unit("kg N/kg C/d").help("N uptake efficiency");
    Input(soilN50).equals(0.036).help("Soil N for 50% maximum N uptake");
    Input(rootC).imports("coffee[rootC]");
    Input(soilNitrogen).imports("soil/nitrogen[value]");
    Input(timeStep).imports("calendar[timeStepDays]");
    Output(value).unit("kg N/m2").help("Nitrogen uptaken from soil");
}

void NitrogenSupply::update() {
    value = rootC*efficiency*soilNitrogen/(soilN50 + soilNitrogen)*timeStep;
}

} //namespace

