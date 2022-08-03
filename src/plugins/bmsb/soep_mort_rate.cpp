/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "soep_mort_rate.h"

using namespace base;

namespace bmsb {

PUBLISH(SOEPMortRate)

SOEPMortRate::SOEPMortRate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimate stage-specific daily mortality rate");
        Input(a0).equals(12).help("first parameter in the second order exponential model");
        Input(a1).equals(1).help("second parameter in the second order exponential model");
        Input(a2).equals(3).help("third parameter in the second order exponential model");
        Input(dailytempC).equals(3).help("third parameter in the second order exponential model");
        Output(mortrate);
}

void SOEPMortRate::reset() {
        update();
}

void SOEPMortRate::update() {
        mortrate = exp(a0 + a1* dailytempC + a2 * (dailytempC)*(dailytempC));
}


}
