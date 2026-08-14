/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "light_absorption_canopy.h"

using namespace base;
using phys_math::PI;

namespace saccharina {

PUBLISH(LightAbsorptionCanopy)

LightAbsorptionCanopy::LightAbsorptionCanopy(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the light absorbed by the canopy");
    Input(I).imports("env[I]");
    Input(crownZoneArea).imports("area[crownZoneArea]");
    Input(plantDensity).imports("area[plantDensity]");
    Input(lai).imports("area[lai]");
    Input(frondAngle).imports("area[frondAngle]");
    Output(value).unit("mu mol PAR / s").help("Radiation absorbed per plant");
    Output(propAbsorbed).unit("[0;1]").help("Proportion of light absorbed");
}


void LightAbsorptionCanopy::reset() {
   update();
}

void LightAbsorptionCanopy::update() {
    const double k = cos(frondAngle/180.*PI);
    propAbsorbed = 1. - exp(-k*lai);
    value = I*crownZoneArea/plantDensity*propAbsorbed;
}

}
