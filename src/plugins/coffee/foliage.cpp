/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "foliage.h"

using namespace base;
using namespace std;

namespace coffee {

PUBLISH(Foliage)

Foliage::Foliage(QString name, QObject *parent)
    : Box(name, parent) {
    help("models coffee leaves");
    Input(slaMax).equals(27.0).help("Maximum specific leaf area");
    Input(fSlaMin).equals(0.64).unit("[0;1]").help("Lower bound of specific leaf area, relative to slaMax");
    Input(transpirationRatio).imports("coffee/waterFlux[transpirationRatio]");
    Input(gLeafC).imports("growth[gLeafC]");
    Output(sla).help("Specific leaf area");
    Output(gLai).help("Growth rate of leaf area index");
}

void Foliage::update() {
    sla = slaMax*(fSlaMin + transpirationRatio*(1 - fSlaMin));
    gLai = sla*gLeafC;
}

} //namespace

