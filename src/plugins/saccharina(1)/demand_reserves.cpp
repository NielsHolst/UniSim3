/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "demand_reserves.h"

using namespace base;

namespace saccharina {

PUBLISH(DemandReserves)

DemandReserves::DemandReserves(QString name, Box *parent)
    : Box(name, parent)
{
    help("calculates demand for reserves");
    Input(biomassTotal).imports("plant[biomassTotal]");
    Input(massCStruct).imports("plant[massCStruct]");
    Input(massCRes).imports("plant[massCRes]");
    Input(massNStruct).imports("plant[massNStruct]");
    Input(massNRes).imports("plant[massNRes]");
    Input(propCRes).imports("plant[propCRes]");
    Input(propNRes).imports("plant[propNRes]");
    Input(propCStruct).imports("plant[propCStruct]");
    Input(optCConc).imports("plant[optCConc]");
    Input(optNConc).imports("plant[optNConc]");
    Input(demandStructGrowthC).imports("../growth[carbon]");
    Input(demandStructGrowthN).imports("../growth[nitrogen]");
    Output(carbon).unit("g C/m").help("Carbon demand for reserves");
    Output(nitrogen).unit("g N/m").help("Nitrogen demand for reserves");
}

void DemandReserves::reset() {
   update();
}

void DemandReserves::update() {
    double
        biomassStructGrowth = demandStructGrowthC/propCStruct,
        a = optCConc*(propNRes*(biomassTotal+biomassStructGrowth) - massNStruct - massNRes - demandStructGrowthN),
        d = optNConc*(propCRes*(biomassTotal+biomassStructGrowth) - massCStruct - massCRes - demandStructGrowthC),
        b = (optNConc - propNRes)*(massCStruct + massCRes + demandStructGrowthC),
        e = (optCConc - propCRes)*(massNStruct + massNRes + demandStructGrowthN),
        c = propCRes*(propNRes - optNConc) - propNRes*optCConc,
        f = propNRes*(propCRes - optCConc) - propCRes*optNConc;
    carbon   = propCRes*(a+b)/c;
    nitrogen = propNRes*(d+e)/f;
}

}
