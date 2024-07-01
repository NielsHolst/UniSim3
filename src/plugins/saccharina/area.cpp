/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "area.h"

using namespace base;

namespace saccharina {

PUBLISH(Area)

Area::Area(QString name, Box *parent)
    : Box(name, parent)
{
    help("converts biomass to plant-related areas");
    Input(biomass).unit("g dw/m").help("Dry weight on which to compute areas");
    Input(ca).unit("leaf m2/g dw").help("Scaling of dry weight to leaf area");
    Input(ea).equals(0.667).help("Scaling exponent of dry weight to leaf area");
    Input(cw).unit("m/g dw").help("Scaling of dry weight to crown zone width");
    Input(ew).equals(0.5).help("Scaling exponent of dry weight to crown zone width");
    Input(cz).unit("m").help("Scaling of crown zone width to crown zone area");
    Output(leaf).unit("leaf m2/m").help("Leaf area");
    Output(crownZoneWidth).unit("/m").help("Crown zone width");
    Output(crownZone).unit("crown zone m2/m").help("Crown zone area");
    Output(index).unit("leaf m2/crown zone m2").help("Leaf area index");
}

void Area::reset() {
    update();
}

void Area::update() {
    if (biomass > 0.) {
        leaf      = ca*pow(biomass, ea);
        crownZoneWidth    = cw*pow(biomass, ew);
        crownZone = cz*crownZoneWidth;
        index     = leaf/crownZone;
    }
    else {
        leaf = crownZoneWidth = crownZone = index = 0.;
    }
}

}
