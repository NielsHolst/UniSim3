/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "light_absorption_leaf.h"

using namespace base;

namespace saccharina {

PUBLISH(LightAbsorptionLeaf)

LightAbsorptionLeaf::LightAbsorptionLeaf(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the light absorbed by a leaf");
    Input(I).imports("env[I]");
    Input(area).imports("area[value]");
    Output(value).unit("mu mol PAR / s").help("Radiation absorbed per plant");
}


void LightAbsorptionLeaf::reset() {
   update();
}

void LightAbsorptionLeaf::update() {
    value = I*area/100.;  // area: dm2/(dm2/m2)= m2
}

}
