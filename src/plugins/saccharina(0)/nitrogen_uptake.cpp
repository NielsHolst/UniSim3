/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "nitrogen_uptake.h"

using namespace base;

namespace saccharina {

PUBLISH(NitrogenUptake)

// 1 umol N = 14 ug N

NitrogenUptake::NitrogenUptake(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates the nitrogen uptake rate");
    Input(beta).equals(10.).unit("g N/dm2/(mmol N/L)/h").help("Effiency of nitrogen uptake");
    Input(demand).imports("demand/nitrogenTotal[value]");
    Input(area).imports("area[value]");
    Input(fCurrent).imports("./fCurrent[value]");
    Input(N).imports("env[N]").unit("mmol N/L").help("Water nitrogen concentration");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Input(dryWeight).imports("biomass[dryWeight]");
    Output(supply).unit("g N").help("Nitrogen supply");
    Output(sdRatio).unit("[0;1]").help("Nitrogen Supply/Demand ratio");
    Output(J).unit("g N/dm2/h").help("J of Broch et al. (2011)");
    Output(V).unit("ug N/g DW/d").help("V of Forbord et al. (2021)");
    Output(B).unit("ug N/g DW/d/umol N").help("B of Forbord et al. (2021)");
}

void NitrogenUptake::reset() {
   update();
   // biomass[dryWeight] has not yet been set
   V = 0.;
}

    void NitrogenUptake::update() {
        double dt = timeStepSecs/3600.;
    supply = (demand<1e-16) ?
                0. :
                demand*(1. - exp(-beta*area*N*fCurrent*dt/demand));
    sdRatio = (demand==0.) ? 0. : supply/demand;
    J = supply/area/dt;
    V = supply/dryWeight/(dt/24)*1000000;
    B = V/(N*1000);
}

}
