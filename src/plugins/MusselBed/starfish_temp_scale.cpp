/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/

/* ##STARFISH SEARCH TEMPERATURE SCALING: this function scales starfish mussel biomass demand to the current
   temperature at step. Function yielded from Antonio AgÃƒÂ¼era, et al. 2012. Winter feeding activity of the common starfish (Asterias rubens L.):
   The role of temperature and shading, Journal of Sea Research, Volume 72, August 2012, Pages 106-112,
   (http://www.sciencedirect.com/science/article/pii/S1385110112000159)*/

#include <base/publish.h>
#include "starfish_temp_scale.h"

using namespace base;

namespace MusselBed {

PUBLISH(StarfishTempScale)

StarfishTempScale::StarfishTempScale(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(a).equals(0.528).help("Regression coefficient");
    Input(b).equals(-3.967).help("Regression coefficient");
    Input(temperature).equals(11).help("current temperature degree celsius");
    Output(value).help("scaling factor for feeding rate");
}

void StarfishTempScale::reset() {
    update();
}

void StarfishTempScale::update() {
    value = exp(a*temperature + b)/(1. + exp(a*temperature + b));
}

} //namespace
