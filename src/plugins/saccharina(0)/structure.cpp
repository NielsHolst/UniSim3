/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "structure.h"

using namespace base;

namespace saccharina {

PUBLISH(Structure)

Structure::Structure(QString name, QObject *parent)
    : Box(name, parent)
{
    help("keeps track of structural mass");
    Input(proportionC).equals(0.21).unit("[0;1]").help("Proportion of the structural mass that is carbon");
    Input(proportionN).equals(0.015).unit("[0;1]").help("Proportion of the structural mass that is nitrogen");
    Input(allocation).unit("g").help("Allocated increment in mass");
    Input(initMass).equals(0.00125).unit("g").help("Initial structural mass");
    Output(mass).unit("g").help("Structural mass");
}

void Structure::reset() {
    mass = initMass;
}

void Structure::update() {
    mass += allocation;
}

}
