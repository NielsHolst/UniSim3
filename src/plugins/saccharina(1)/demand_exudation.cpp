/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "demand_exudation.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandExudation)

DemandExudation::DemandExudation(QString name, Box *parent)
    : Box(name, parent)
{
    help("calculates demand for exudation");
    Input(relExudation).equals(0.23).unit("g C/g C").help("Relative exudation cost");
    Input(demandGrowthC).imports("../growth[carbon]");
    Input(demandCRes).imports("../reserves[carbon]");
    Output(carbon).unit("g C/m").help("Carbon demand for exudation");
}

void DemandExudation::reset() {
   update();
}

void DemandExudation::update() {
    carbon = relExudation*(demandGrowthC+demandCRes);
}

}
