/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "carbon_reserves_max.h"

using namespace base;

namespace saccharina {

PUBLISH(CarbonReservesMax)

CarbonReservesMax::CarbonReservesMax(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates maximum carbon reserves");
    Input(youngMax).unit("g C per g structure").equals(0.23).help("Maximum reserves of young plants");
    Input(matureMax).unit("g C per g structure").equals(1.10).help("Maximum reserves of mature plants");
    Input(maturity).imports("maturity[value]");
    Output(value).unit("per hour").help("Age-corrected maximum carbon reserves");
}

void CarbonReservesMax::reset() {
   update();
}

void CarbonReservesMax::update() {
    value = youngMax + maturity*(matureMax - youngMax);
}

}

