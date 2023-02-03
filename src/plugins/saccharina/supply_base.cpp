/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "supply_base.h"

using namespace base;

namespace saccharina {

SupplyBase::SupplyBase(QString name, Box *parent)
    : Box(name, parent)
{
    help("is a base class for computing supply in response to demand");
    Input(demand).unit("g/m/d").help("Demand for C, N or P");
    Output(value).unit("g/m/d)").help("Supply of C, N or P");
    Output(sdRatio).unit("[0;1]").help("Supply/demand ratio");
}

void SupplyBase::reset() {
    update();
}

void SupplyBase::update() {
    if (demand > 0.) {
        updateValue();
        sdRatio = value/demand;
    }
    else {
        value =
        sdRatio = 0.;
    }
}

}
