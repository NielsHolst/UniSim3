/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "carbon_init_limits.h"

using namespace base;

namespace saccharina {

PUBLISH(CarbonInitLimits)

CarbonInitLimits::CarbonInitLimits(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates initial limits for carbon reserves from g");
    Input(Pmax).imports("parameters/Pmax[value]");
    Input(slope).equals(-1750.).help("Cinit on Pmax");
    Input(intercept).equals(2.95).help("Intercept of Cinit on Pmax");
    Output(min).help("Minimum Cinit");
}

void CarbonInitLimits::reset() {
    update();
}

void CarbonInitLimits::update() {
    min = slope*Pmax + intercept;
}

}
