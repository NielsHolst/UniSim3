/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/test_num.h>
#include <base/vector_op.h>
#include "honey_supply.h"

using namespace base;
using vector_op::sum;

namespace apis {

PUBLISH(HoneySupply)

HoneySupply::HoneySupply(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes potential honey supply");
    Input(cohortNumbers).help("Forager cohort numbers");
    Input(cropVolume).equals(0.030).help("Volume of forager crop (ml)");
    Input(handlingTime).equals(10).help("Interval between foraging trips (min)");
    Input(conversionFactor).equals(1).help("Conversion factor from nectar to honey (g/ml)");
    Input(nectarFlow).imports("nectarFlow[rate]");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Output(value).help("Supply of honey over this time step (g)");
    Output(rate).help("Honey supply rate (g/h)");
}

void HoneySupply::update() {
    double demandRate = sum(cohortNumbers)*cropVolume/handlingTime, // ml/min
           flowRate = nectarFlow/60., // ml/min
           supplyRate = TestNum::eqZero(demandRate) ? 0 : demandRate*(1. - exp(-flowRate/demandRate));
    // g/h = g/ml * ml/min * min/h
    rate = conversionFactor*supplyRate*60;
    // g = g/h * s * h/s
    value = rate*timeStepSecs/3600.;
}

}
