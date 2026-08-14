/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "supply_nitrogen.h"

using namespace base;

namespace saccharina {

PUBLISH(SupplyNitrogen)

SupplyNitrogen::SupplyNitrogen(QString name, Box *parent)
    : Box(name, parent)
{
    help("calculates supply of nitrogen");
    Input(uptakeRate).equals(20e-6).unit("g N/microM/M2/d").help("Uptake rate of nitrogen");
    Input(area).imports("geometry[area]");
    Input(N).imports("env[N]");
    Input(timeStep).imports("calendar[timeStepDays]");
    Input(demands).imports("demand/*[nitrogen]");
    Output(demand).unit("g N/m").help("Total demand for nitrogen");
    Output(value).unit("g N/m").help("Total supply of nitrogen");
}

void SupplyNitrogen::reset() {
   update();
}

void SupplyNitrogen::update() {
    demand = vector_op::sum(demands);
    value = (demand > 0.) ? demand*(1. - exp(-uptakeRate*area*N*timeStep/demand)) : 0.;
}

}

