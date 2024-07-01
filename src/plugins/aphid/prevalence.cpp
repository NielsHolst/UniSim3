/* Copyright 2018-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "prevalence.h"

using namespace base;

namespace aphid {

PUBLISH(Prevalence)

Prevalence::Prevalence(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes prevalence of exposed aphids and of cadavers");
    Input(aphidDensity).help("Current total density of live aphids").unit("per tiller");
    Input(exposedDensity).help("Current density of exposed aphids").unit("per tiller");
    Input(cadaverDensity).help("Current density of cadavers").unit("per tiller");
    Output(exposed).help("Percentage exposed aphids out of all live aphids").unit("%");
    Output(cadavers).help("Percentage cadavers out of aphids+cadavers").unit("%");
}

void Prevalence::update() {
    exposed = (aphidDensity > 0.) ? 100.*exposedDensity/aphidDensity : 0.;
    double liveAndDead = aphidDensity + cadaverDensity;
    cadavers = (liveAndDead > 0.) ? 100.*cadaverDensity/liveAndDead : 0.;
}

}
