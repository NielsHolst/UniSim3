/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include "topsoil_runoff.h"
#include <cmath>
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (TopsoilRunoff)
	
TopsoilRunoff::TopsoilRunoff(QString name, Box *parent)
	: Box(name, parent)
{
    help("manages fate of pesticide due to runoff in the topsoil");
    Input(S).unit("fraction").equals(0.01).help("Slope of the field");
    Input(wbz).unit("m").equals(1.).help("Buffer zone");
    Input(P).unit("mm").equals(1.).help("Daily rainfall");
    Input(KocpH).unit("L/kg").equals(5.678e-6).help("pH-dependent organic carbon-water partitioning coefficient");
    Input(fom).unit("fraction").equals(0.09).help("Fraction of organic matter in the soil");
    Input(Q).unit("mm").equals(5.).help("Runoff amount");

    Output(F).help("Overall correction factor");
    Output(Fslope).help("Slope correction factor");
    Output(Fbuffer).help("Buffer zone correction factor");
    Output(fsr).unit("fraction").help("Runoff fraction in the topsoil");
}

void TopsoilRunoff::update() {
    if (S > 0.2){
        Fslope = 1;
    }
    else if (S <0.2){
        Fslope = 0.02153*S + 0.001423*pow(S, 2);
    }
    else{
        Fslope = 0.02153 + 0.0014426;
    }
    Fbuffer = pow(0.83, wbz);
    F = Fbuffer * Fslope;
    Kd = KocpH * fom;
    fsr = (P > 0) ? (Q/P) * F * (1./ (1. + Kd)) : 0;
}

} //namespace

