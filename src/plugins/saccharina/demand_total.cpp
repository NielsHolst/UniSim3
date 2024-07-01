/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "demand_total.h"

using namespace base;
using std::max;
using vector_op::sum;

namespace saccharina {

PUBLISH(DemandTotal)

DemandTotal::DemandTotal(QString name, Box *parent)
    : Density(name, parent)
{
    help("computes total demand");
    Input(dws).unit("g dw/m/d").help("Dry weight demands");
    Input(Cs).unit("g C/m/d").help("Carbon demands");
    Input(Ns).unit("g N/m/d").help("Nitrogen demands");
    Input(Ps).unit("g P/m/d").help("Nitrogen demands");
    Input(biomass).unit("g dw/m/d").help("Plant biomass");
    Input(concP).unit("g P/g dw").help("Plant phosphorus concentration");
    Input(concPmax).unit("g P/g dw").help("Max. phosphorus concentration");
}

void DemandTotal::reset() {
   update();
}

void DemandTotal::update() {
    dw = sum(dws);
    C = sum(Cs);
    N = sum(Ns);
    P = max((biomass + dw)*concPmax - biomass*concP, 0.);
}

}
