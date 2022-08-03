/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/test_num.h>
#include "beta_limits.h"

using namespace base;

namespace saccharina {

PUBLISH(BetaLimits)

BetaLimits::BetaLimits(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates beta limits from Pmax and g");
    Input(g).imports("parameters/g[value]");
    Input(kg).imports("parameters[kg] ").unit("-").help("Relative faster growth rate of young plants");

    Input(Pmax).imports("parameters/Pmax[value]");
    Input(gSlopeFloor).equals(0.25).help("Slope of beta on g (floor line)");
    Input(gInterceptFloor).equals(-0.0008).help("Intercept of beta on g (floor line)");

    Input(gSlopeCeiling).equals(0.35).help("Slope of beta on g (ceiling line)");
    Input(gInterceptCeiling).equals(-0.0010).help("Intercept of beta on g (ceiling line)");

    Input(PmaxSlope).equals(4.17).help("Slope of beta on Pmax");
    Input(PmaxIntercept).equals(-0.0002).help("Intercept of beta on Pmax");
    Output(min).help("Minimum beta");
    Output(max).help("Maximum beta");
}

void BetaLimits::reset() {
    update();
}

void BetaLimits::update() {
    double
        gFloor = g*kg*gSlopeFloor + gInterceptFloor,
        gCeiling = g*kg*gSlopeCeiling + gInterceptCeiling,
        PmaxCeiling = Pmax*PmaxSlope + PmaxIntercept;
    min = gFloor;
    max = std::min(gCeiling, PmaxCeiling);
    if (TestNum::ge(min,max))
        max = (1+1e-6)*min;
}

}
