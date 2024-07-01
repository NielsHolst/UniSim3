/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include "topsoil_degradation.h"
#include <cmath>
#include <base/publish.h>
#include "general.h"

using namespace base;

namespace PestTox {

PUBLISH (TopsoilDegradation)
	
TopsoilDegradation::TopsoilDegradation(QString name, Box *parent)
	: Box(name, parent)
{
    help("manages fate of pesticide in the topsoil");
    Input(inflow).unit("g a.i/ha").equals(0.).help("Inflow as pesticide deposition in the soil");
    Input(DT50).unit("day").equals(10.).help("Biodegradation half-life");
    Input(Sp).equals(0.).help("Soil porosity");
    Input(Tsoil).unit("Â°C").equals(25.).help("Soil temperature");
    Input(Vrsmc).equals(0.5).help("Reference soil moisture content");
    Input(rf).equals(0.7).help("Exponential response factor for the influence of moisture content on biodegradation rate");
    Input(fw).unit("fraction").equals(0.25).help("Fraction of water in the soil");
    Input(Rainfall).unit("mm").equals(0.).help("Daily rainfall");
    Input(leafWashOff).unit("g a.i/ha").equals(0.).help("Pesticide washed off from crop or leaf surface");
    Input(ksv).unit("per day").equals(0.).help("Volatilization rate constant");
    Input(frsa).unit("fraction").equals(0.).help("Fraction of pesticide reaching soil air");
    Input(frsw).unit("fraction").equals(0.).help("Fraction of pesticide reaching soil water");
    Input(fsr).unit("fraction").equals(0.).help("Runoff fraction in the topsoil");
    Input(fmp).unit("fraction").equals(0.).help("Macropore fraction");

    Output(concentration).unit("g a.i/ha").help("Pesticide concentration in the topsoil");
    Output(outflow).unit("g a.i/ha").help("Pesticide loss in the topsoil");
    Output(fsdT).help("Temperature correction factor");
    Output(Vsmc).help("Soil moisture content");
    Output(fsm).help("Moisture correction factor");
    Output(ksd).unit("per day").help("Moisture and temperature-corrected biodegradation rate constant");
    Output(degradation).unit("g a.i/ha").help("Pesticide loss by biodegration");
    Output(volatilization).unit("g a.i/ha").help("Pesticide loss by volatilization");
    Output(runoff).unit("g a.i/ha").help("Pesticide loss by runoff");
    Output(macroporeFlow).unit("g a.i/ha").help("Pesticide loss by macropore flow");
}

void TopsoilDegradation::update() {
    const double Tref = 25.;
    double k = log(2)/DT50;
    fsdT = (Tsoil > 0) ? pow(Q10,(Tsoil - Tref)/10.) : 0;
    Vsmc = (Sp > 0) ? fw/Sp : 0;
    fsm = (Vsmc >= Vrsmc) ? 1 : pow((Vsmc/Vrsmc),rf);
    ksd = k*fsdT*fsm;

    outflow = (concentration*ksd)+(ksv*(frsa+frsw)*concentration)
            +(fsr*concentration)+(fmp*concentration);
    concentration += (inflow + leafWashOff) - outflow;
    degradation = concentration*ksd;
    volatilization = ksv*(frsa+frsw)*concentration;
    runoff = fsr*concentration;
    macroporeFlow = fmp*concentration;
}

} //namespace

