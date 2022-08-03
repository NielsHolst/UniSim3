/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "loss_rate.h"
#include <cmath>
#include <base/publish.h>


using namespace base;

namespace PestTox {
	
LossRate::LossRate(QString name, Box *parent)
	: Box(name, parent)
{
    help("manages instantaneous loss rate");
    Output(instantaneous).unit("per s").help("Instantaneous loss rate");
    Output(finite).unit("per day").help("Finite loss rate");
}

void LossRate::reset() {
    update();
}

void LossRate::update() {
    instantaneous = computeInstantaneous();
    finite = 1. - exp(-24*60*60*instantaneous);
}

} //namespace

