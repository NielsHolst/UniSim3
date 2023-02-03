/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "plant_compartment.h"

using namespace base;

namespace saccharina {

PUBLISH(PlantCompartment)

PlantCompartment::PlantCompartment(QString name, Box *parent)
    : Density(name, parent)
{
    help("models a plant compartment");
    Input(concC).unit("g C/g dw").help("Carbon concentration");
    Input(concN).unit("g N/g dw").help("Nitrogen concentration");
    Input(concP).unit("g P/g dw").help("Phosphorus concentration");
    Input(dwInit).unit("g dw/m").help("Initial total dry weight");
    Input(dwIncrement).unit("g dw/m").help("Increment of total dry weight");
}

void PlantCompartment::reset() {
    dw = dwInit;
    update();
}

void PlantCompartment::update() {
    dw += dwIncrement;
    if (dw < 0.)
        dw = 0.;
    C = dw*concC;
    N = dw*concN;
    P = dw*concP;
}

}
