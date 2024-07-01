/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include "leaf_wash_off.h"
#include <cmath>
#include <base/publish.h>


using namespace base;

namespace PestTox {

PUBLISH (LeafWashOff)
	
LeafWashOff::LeafWashOff(QString name, Box *parent)
    : LossRate(name, parent)
{
    help("manages fate of pesticide on crop or leaf surface due to washoff");
    Input(rainfall).unit("mm").imports("weather[Rain]").help("Daily rainfall");
    Input(washOffCoefficient).unit("per mm").help("Wash-off coefficient");
}

double LeafWashOff::computeInstantaneous() {
    double dailyRate = 1. - exp(-washOffCoefficient*rainfall);  // per day
    return -log(1. - dailyRate)/24./3600.;  // per s
}

} //namespace

