/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "honey_demand_respiration.h"

using namespace base;
using vector_op::sum;

namespace apis {

PUBLISH(HoneyDemandRespiration)

HoneyDemandRespiration::HoneyDemandRespiration(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes honey demand for adult respiration");
    Input(workers).imports("houserkeeper[number]|forager[number]").transform(Sum).help("Number of workers");
    Input(respirationRate).equals(7).help("Honey used for worker bee respiration (mg/d/bee");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Output(value).help("Respiration demand over this time step (g)");
    Output(rate).help("Respiration demand rate (g/h)");
    Output(total).help("Total mass of honey used for respiration (g");
}

void HoneyDemandRespiration::reset() {
    total = 0;
}

void HoneyDemandRespiration::update() {
    rate = workers*respirationRate/1000./24.;
    value = rate*timeStepSecs/3600.;
    total += value;
}

}
