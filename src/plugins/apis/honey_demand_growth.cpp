/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "honey_demand_growth.h"

using namespace base;
using namespace vector_op;

namespace apis {

PUBLISH(HoneyDemandGrowth)

HoneyDemandGrowth::HoneyDemandGrowth(QString name, QObject *parent)
    : Box(name, parent)
{
    help("computes honey demand for juvenile growth");
    Input(cohortNumbers).help("Larva cohort numbers");
    Input(cohortDemands).help("Larva cohort growth demands (mg)");
    Input(cost).equals(0.3).help("Relative cost per unit of biomass browth");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Output(rate).help("Growth demand rate, including conversion cost (g/h)");
    Output(value).help("Growth demand over this time step, including conversion cost (g)");
    Output(netTotal).help("Total mass of honey converted to biomass growth (g)");
    Output(costTotal).help("Total mass of honey used for biomass conversion cost (g)");
}

void HoneyDemandGrowth::reset() {
    netTotal = costTotal = 0.;
}

void HoneyDemandGrowth::update() {
    QVector<double> cohortDemandsTotal;
    product(cohortDemandsTotal,
            cohortNumbers, cohortDemands);

    value = sum(cohortDemandsTotal)/(1-cost)/1000.;
    rate = value/timeStepSecs*3600.;
    netTotal += value*(1-cost);
    costTotal += value*cost;
}

}
