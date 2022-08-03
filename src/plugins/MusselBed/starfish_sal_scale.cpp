/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/

/* ##STARFISH FEEDING SALINITY SCALING: this function scales starfish mussel biomass demand to the current
   salinity at step. This expression is based in field observations in mesocosm experiment at the laboratory*/

#include <base/publish.h>
#include "starfish_sal_scale.h"

using namespace base;

namespace MusselBed {

PUBLISH(StarfishSalScale)

StarfishSalScale::StarfishSalScale(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(a).equals(-16.831).help("Regression coefficient");
    Input(b).equals(0.743).help("Regression coefficient");
    Input(salinity).equals(28).help("current salinity");
    Output(value).help("scaling factor for feeding rate");
}

void StarfishSalScale::reset() {
    update();
}

void StarfishSalScale::update() {
    value = exp(a + b*salinity)/(1+exp(-a + b*salinity));
}

} //namespace

