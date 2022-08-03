/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "senescence.h"

using namespace base;
using namespace std;

namespace coffee {

PUBLISH(Senescence)

Senescence::Senescence(QString name, QObject *parent)
    : Box(name, parent) {
    Input(tccLmax).equals(650.).help("Maximum leaf lifespan");
    Input(ftccLmin).equals(0.64).help("Proportional minimum leaf lifespan");
    Input(tccR).help("Average lifespan of roots");
    Input(transpirationRatio).imports("coffee/waterFlux[transpirationRatio]");
    Input(lai).imports("/*/coffee[lai]");
    Input(rootC).imports("coffee[rootC]");
    Input(leafC).imports("coffee[leafC]");
    Input(leafN).imports("coffee[leafN]");
    Output(dRootC).help("Loss rate of root carbon");
    Output(dLeafN).help("Loss rate of leaf nitrogen");
    Output(dLeafC).help("Loss rate of leaf carbon");
    Output(dLai).help("Loss rate of leaf area index");
}

void Senescence::update() {
    double leafLifespan = tccLmax*(transpirationRatio + ftccLmin*(1-transpirationRatio)),
           leafLossRate = min(1/leafLifespan, 1.);
    dRootC = rootC/tccR;
    dLeafN = leafN*leafLossRate;
    dLeafC = leafC*leafLossRate;
    dLai = lai*leafLossRate;
}

} //namespace

