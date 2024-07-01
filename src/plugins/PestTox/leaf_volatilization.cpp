/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <cmath>
#include <base/phys_math.h>
#include <base/publish.h>
#include "leaf_volatilization.h"

using namespace base;
using namespace phys_math;

namespace PestTox {
	
PUBLISH(LeafVolatilization)

LeafVolatilization::LeafVolatilization(QString name, Box *parent)
    : LossRate(name, parent)
{

    help("manages fate of pesticide on crop or leaf surface due to volatilization");
    Input(VP).unit("Pa").imports("applications[vapourPressure]").help("Vapour pressure of the pesticide at reference temperature");
    Input(Ea).unit("kJ/mol").imports("applications[activationEnergyVolatilization]").help("Activation energy");
    Input(Tref).unit("Â°C").imports("applications[Tref]").help("Reference temperature");
    Input(Tair).unit("Â°C").imports("weather[Tavg]").help("Daily average air temperature");
    Input(load).unit("g a.i/ha").imports("onCrop[load]").help("Current concentration of the pesticide on crop or leaf surface");

    Output(Tcorrection).unit("scalar").help("Temperature correction factor");
    Output(evaporationRate).unit("g/ha/h").help("Evaporation rate from leaves");
}

double LeafVolatilization::computeInstantaneous() {

    double evaporationRateTref = exp(12.2 + 0.933 * log(VP)) * 1e-6 * 1e4;  // g a.i/ha/h = myg a.i/m2/h * g a.i/myg * m2/ha
    Tcorrection = Ea*1000./R*((1./(Tref + T0)) - 1./(Tair + T0));           // 1 = kJ/mol * J/KJ / (J/mol/K) / K
    evaporationRate = evaporationRateTref * exp(Tcorrection) / 3600;        // g a.i/ha/s = g a.i/ha/h / (s/h)
    return (load > 0) ? evaporationRate/load : 0.;                          // s-1 = g a.i/ha/s / (g a.i/ha)
  }

} //namespace

