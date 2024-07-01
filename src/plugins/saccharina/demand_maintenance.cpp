/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "demand_maintenance.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandMaintenance)

DemandMaintenance::DemandMaintenance(QString name, Box *parent)
    : Density(name, parent)
{
    help("computes maintenance respiration");
    Input(biomass).unit("g dw/m").help("Total dry weight");
    Input(T).unit("oC").help("Temperature");
    Input(Q10).equals(1.05).help("Temperature response");
    Input(r20).equals(10.).unit("mg glucose/g dw/d").help("Respiration rate at 20 oC");
}

void DemandMaintenance::reset() {
   update();
}

void DemandMaintenance::update() {
    double rT = r20/1000.*pow(Q10, (T-20.)/10.);
    dw = N = 0;
    C = 72./180.*rT*biomass;
}

}
