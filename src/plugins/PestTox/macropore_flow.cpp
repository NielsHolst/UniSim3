/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "macropore_flow.h"
#include <cmath>
#include <base/publish.h>



using namespace base;

namespace PestTox {

PUBLISH (MacroporeFlow)
	
MacroporeFlow::MacroporeFlow(QString name, Box *parent)
	: Box(name, parent)
{
    help("manages fate of pesticide due to macropore flow");
    Input(fsand).unit("fraction").equals(0.25).help("Fraction of sand in the soil");
    Input(fsilt).unit("fraction").equals(0.25).help("Fraction of silt in the soil");
    Input(fclay).unit("fraction").equals(0.5).help("Fraction of clay in the soil");
    Input(fa).unit("fraction").equals(0.25).help("Fraction of air in the soil");
    Input(fw).unit("fraction").equals(0.25).help("Fraction of water in the soil");
    Input(fmacropore).unit("fraction").equals(0.3).help("Fraction of macropore i.e. pore fractions which acts as macropores");
    Input(Flowrate).unit("m/day").equals(0.35).help("Flow rate");
    Input(frr).unit("fraction").equals(0.5).help("Fraction of rain runoff");
    Input(tpevent).unit("h").equals(5.).help("Number of hours precipitation occurs on a rainy day");
    Input(P).unit("mm").equals(1.).help("Daily rainfall");
    Input(frsw).unit("fraction").equals(0.).help("Fraction of pesticide in soil water");
    Input(Tf).equals(0.).help("Tillage factor");

    Output(fporemobile).unit("fraction").help("Mobile pore fraction in topsoil");
    Output(Vporeimmobile).unit("m3/m3").help("Immobile pore volume in topsoil");
    Output(C).unit("m/h").help("Storage capacity");
    Output(Cused).unit("m/h").help("Used capacity");
    Output(Cfree).unit("m/h").help("Free capacity");
    Output(Pcapacity).unit("mm/h").help("Rain capacity or freecapacity recalculated as the amount of rain that can be taken up in 1 hour");
    Output(Preqh).unit("mm/h").help("Rainfall required before macropore flow starts");
    Output(Preqday).unit("mm/day").help("Rainfall required before macropore flow starts");
    Output(Pi).help("Integrated distribution of precipitation");
    Output(fmp).unit("fraction").help("Fraction of macropore flow");
    Output(Vporemobile).help("Mobile pore volume in topsoil");
}

void MacroporeFlow::update() {
    fporemobile = 0.72 * fsand + 0.35 * fsilt + 0.14 * fclay;
    Vporemobile = fporemobile * (fw * fa);
    Vporeimmobile = (fw + fa)- Vporemobile;
    C = Vporeimmobile * 0.01 + ((1. - fmacropore) * Vporemobile *Flowrate/24.);
    Cused = (fw /(fw + fa))* C;
    Cfree = C - Cused;
    Pcapacity = 1000. * Cfree;
    Preqh = Pcapacity /(1. - frr);
    Preqday = Preqh * 24.;
    Pi = (P == 0.) ? 0 : exp(-tpevent /(24.*P)*Preqday);
    fmp = Pi*frsw*(Tf/7.5);
}

} //namespace

