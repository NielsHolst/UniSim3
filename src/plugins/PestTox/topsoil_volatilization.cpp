/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <cmath>
#include <base/publish.h>
#include "general.h"
#include "topsoil_volatilization.h"

using namespace base;

namespace PestTox {

PUBLISH (TopsoilVolatilization)
	
TopsoilVolatilization::TopsoilVolatilization(QString name, Box *parent)
	: Box(name, parent)
{
    help("manages topsoil volatilization");
    Input(Tair).unit("Â°C").imports("weather[Tavg]").help("Daily average air temperature");
    Input(MV).unit("cm3/mol").equals(140.83).help("Molar volume of the pesticide");
    Input(BLair).unit("m").equals(0.00475).help("Thickness of the air boundary layer for the evaporation");
    Input(Pd).unit("m").equals(0.01).help("Penetration depth");
    Input(fa).unit("fraction").equals(0.25).help("Fraction of air in the soil");
    Input(fw).unit("fraction").equals(0.25).help("Fraction of water in the soil");
    Input(Pair).unit("atm").equals(1.).help("Gas phase pressure of the air");
    Input(farea).unit("m2").equals(100.).help("Area of the whole field");
    Input(Za).unit("mol/m3/Pa").equals(0.).help("Z-value of pesticide in air phase");
    Input(Zw).unit("mol/m3/Pa").equals(0.).help("Z-value of pesticide in water phase");
    Input(VZ).equals(0.).help("Sum of VZ-value of all three phase i.e. air, water, and solid phases");
    Input(MW).unit("g/mol").equals(169.07).help("Molar mass of the pesticide");
    Input(viscosity).unit("centipoise").equals(0.).help("Viscosity of water");

    Output(Ba).unit("m2/s").help("Molecular diffusivity of the pesticide in air");
    Output(Dbl).unit("mol/m3/day").help("D-value of the pesticide diffusion through the air boundary layer");
    Output(Bea).unit("m2/s").help("Effective diffusivity of the pesticide in air");
    Output(Bew).unit("m2/s").help("Effective diffusivity of the pesticide in water");
    Output(Da).unit("mol/m3/day").help("D-value of the pesticide in air");
    Output(Dw).unit("mol/m3/day").help("D-value of the pesticide in water");
    Output(Dv).unit("mol/m3/day").help("Total D-value of the volatilization process");
    Output(ksv).unit("per day").help("Volatilization rate constant");
    Output(Bw).unit("m2/s").help("Molecular diffusivity of selected pesticide in water");
    Output(Dpl).unit("m").help("Diffusion path length");
    Output(ke).unit("m/s").help("Transport velocity through boundary layer");
}

void TopsoilVolatilization::update() {
    Ba = 1./10000. * 1./1000. * (pow((Tair + 273.15), 1.75)*sqrt(((1./MWair) + (1./MW))) / (Pair*pow((pow(MVair, 1./3.) + pow(MV, 1./3.)), 2)));
    ke = Ba / BLair;
    Dbl = 3600. * 24. * ke * farea * Za;
    Bea = Ba*pow(fa, 10./3.)/pow((fa+fw), 2);
    Bw = (1./10000.)*0.0001326/(pow(viscosity, 1.14)* pow(MV, 0.589));
    Bew = Bw*pow(fw, 10./3.)/pow((fa+fw), 2);
    Dpl = Pd / 2.;
    Da = 3600. * 24. * Bea * farea * Za / Dpl;
    Dw = 3600. * 24. * Bew * farea * Zw / Dpl;
    Dv = 1./((1./Dbl)+(1./(Da + Dw)));
    ksv = Dv/VZ;
}

} //namespace

