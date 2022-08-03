/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "supply_carbon.h"

using namespace base;

namespace saccharina {

PUBLISH(SupplyCarbon)

SupplyCarbon::SupplyCarbon(QString name, Box *parent)
    : Box(name, parent)
{
    help("calculates supply of carbon");
    Input(concN).imports("plant[concN]");
    Input(k).equals(0.7).unit("-").help("Light extinction coefficient");
    Input(lai).imports("geometry[lai]");
    Input(crownZoneArea).imports("geometry[crownZoneArea]");
    Input(a).equals(0.0251).unit("g C/mol PAR").help("Efficiency at zero [N]");
    Input(b).equals(2.62).unit("-").help("Slope of efficiency on [N]");
    Input(calib).equals(1.);
//    Input(calib).equals(1.); //.help("Calibration multiplier on efficiency);
    Input(E0).imports("env[E0]");
    Input(demands).imports("demand/*[carbon]");
    Output(Iabs).unit("mol PAR/m").help("Absorbed light");
    Output(efficiency).unit("g C/mol PAR").help("Photosynthetic efficiency");
    Output(demand).unit("g C/m").help("Total demand for carbon");
    Output(value).unit("g C/m").help("Total supply of carbon");
}

void SupplyCarbon::reset() {
   update();
}

void SupplyCarbon::update() {
    Iabs = (1. - exp(-k*lai))*crownZoneArea*E0;
    demand = vector_op::sum(demands);
    efficiency = calib*(a + b*concN);
    value = (demand > 0.) ? demand*(1. - exp(-efficiency*Iabs/demand)) : 0.;
}

}

