/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <cmath>
#include <base/phys_math.h>
#include <base/publish.h>
#include "zvalues.h"

using namespace base;
using namespace phys_math;

namespace PestTox {

PUBLISH (ZValues)
	
ZValues::ZValues(QString name, Box *parent)
	: Box(name, parent)
{
    help("manages zvalues of the pesticide");
    Input(MW).unit("g/mol").equals(169.07).help("Molar mass of the pesticide");
    Input(Pd).unit("m").equals(0.01).help("Penetration depth");
    Input(VP).unit("Pa").equals(2.45e-5).help("Vapour pressure at reference temperature");
    Input(Psol).unit("g/L").equals(12.).help("Solubility of the pesticide");
    Input(foc).unit("fraction").equals(0.19).help("Fraction of organic carbon in the topsoil");
    Input(pb).unit("kg/L").equals(1.1).help("Soil bulk density");
    Input(fa).unit("fraction").equals(0.25).help("Fraction of air in the soil");
    Input(fw).unit("fraction").equals(0.25).help("Fraction of water in the soil");
    Input(fs).unit("fraction").equals(0.5).help("Fraction of solids in the soil");
    Input(KocpH).unit("L/kg").equals(5.678e-6).help("pH-dependent organic carbon-water partitioning coefficient");
    Input(Tair).unit("Â°C").imports("weather[Tavg]").help("Daily average air temperature");
    Input(R).unit("J/mol/K").equals(8.3145).help("Gas constant");
    Input(Sp).equals(0.5).help("Soil porosity");
    Input(farea).unit("m2").equals(100.).help("Area of the whole field");

    Output(Za).unit("mol/m3/Pa").help("Z-value of pesticide in air phase");
    Output(Zw).unit("mol/m3/Pa").help("Z-value of pesticide in water phase");
    Output(Zs).unit("mol/m3/Pa").help("Z-value of pesticide in solid phase");
    Output(VPTadap).unit("Pa").help("Vapour pressure at actual temperature");
    Output(Vs).unit("m3").help("Volume of solids in the soil volume susceptible for volatilization");
    Output(Vw).unit("m3").help("Volume of water in the soil volume susceptible for volatilization");
    Output(Va).unit("m3").help("Volume of air in the soil volume susceptible for volatilization");
    Output(VZ).help("Sum of VZ-value of all three phase i.e. air, water, and solid phases");
}

void ZValues::update() {
    const double Tref = 25.;
    VPTadap = VP * exp((1./10.)*(Tair - Tref));
    Zw = 1000.*Psol/(VPTadap * MW);
    Za = 1./(R*(Tair + T0));
    Zs = (KocpH *foc * pb * Zw) / (1. - Sp);
    Vs = Pd*fs*farea;
    Vw = Pd*fw*farea;
    Va = Pd*fa*farea;
    VZ = Va*Za + Vw*Zw + Vs*Zs;
}

} //namespace

