/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <cmath>
#include <base/exception.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include "leaf_uptake.h"

using namespace base;
using namespace phys_math;

namespace PestTox {

PUBLISH (LeafUptake)
	
LeafUptake::LeafUptake(QString name, Box *parent)
    : LossRate(name, parent)
{
    help("manages fate of pesticide on crop or leaf surface due to uptake");
    Input(leafType).equals("NonWaxy").help("either 'Waxy' or 'NonWaxy'");
    Input(Tair).unit("Â°C").imports("weather[Tavg]").help("Air temperature");
    Input(MV).imports("applications[molarVolume]").help("Molar volume");
}

double LeafUptake::computeInstantaneous() {

    double V = 0.945*MV + 2.772,
           B = R*2.303*(Tair + T0),
           log_klu;

    if (leafType == "NonWaxy")
        log_klu = -4.93 + 5.30e-3*V - 63.222*exp(2.874*MV/1000)/B;
    else if (leafType == "Waxy")
        log_klu = -5.31 + 5.84e-3*V - 78.875*exp(3.470*MV/1000)/B;
    else
        ThrowException("LeafType must be either 'Waxy' or 'NonWaxy").value(leafType).context(this);

    return pow(10., log_klu);       // per second
}

} //namespace

