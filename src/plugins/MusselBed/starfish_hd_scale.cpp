/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/

/* ##STARFISH FEEDING HYDRODYNAMICS SCALING: this function scales starfish mussel biomass demand to the location Hydrodynamic Regime.
** Function yielded from Antonio AgÃƒÂ¼era, et al. 2014
*/

#include <base/publish.h>
#include "starfish_hd_scale.h"

using namespace base;

namespace MusselBed {

PUBLISH(StarfishHdScale)

StarfishHdScale::StarfishHdScale(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(a).equals(3.873).help("Regression coefficient");
    Input(b).equals(-0.0961).help("Regression coefficient");
    Input(vmax).equals(70).help("maximum flow velocity at location (cm/s)");
    Input(mBiomass).equals(1).help("current mussel biomass, mussel biomass reduce HD's effect");
    Output(scale).help("scaling factor for current hydrodynamics with a density of 1000g/mussel");
    Output(value).help("scaling factor for search rate at current mBiomass and flow");
}

void StarfishHdScale::reset() {
    for (int t=0; t<=180; t++) {
        double x = exp(a+(b*vmax*sin(M_PI*t/360)));
        scale += x/(1+x);
    }
    scale = (scale/180);
    update();
}



void StarfishHdScale::update() {
    if (mBiomass<=5000)
        value = scale + (1-scale)*(mBiomass-1000)/5000;
    else
        value = 1;
}
}
//namespace
