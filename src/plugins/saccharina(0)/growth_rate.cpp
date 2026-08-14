/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "growth_rate.h"

using namespace base;

namespace saccharina {

PUBLISH(GrowthRate)

GrowthRate::GrowthRate(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates optimal plant growth rate");
    Input(gMature).imports("parameters[g] ").unit("per hour").help("Growth rate of mature plants");
    Input(kg).imports("parameters[kg] ").unit("-").help("Relative faster growth rate of young plants");
    Input(maturity);
    Output(value).unit("per hour").help("Age-corrected growth rate");
}

void GrowthRate::reset() {
   update();
}

void GrowthRate::update() {
    value = gMature*kg - gMature*(kg - 1.)*maturity;
}

}

