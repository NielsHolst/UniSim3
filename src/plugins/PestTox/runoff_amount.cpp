/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include "runoff_amount.h"
#include <cmath>
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (RunoffAmount)
	
RunoffAmount::RunoffAmount(QString name, Box *parent)
	: Box(name, parent)
{

    help("manages runoff amount");
    Input(P).unit("mm").equals(1.).help("Daily rainfall");
    Input(fsand).unit("fraction").equals(0.5).help("Fraction of sand in the soil");

    Output(Q).unit("mm").help("Runoff amount");
}

void RunoffAmount::update() {
    // x is for loamy soils and y is for sandy soils
    double x = ((-0.061108 - 0.0041626*P + 0.0040395*pow(P, 2) - 9.0361e-6*pow(P, 3)) < 0) ?
               (-0.0041626*P + 0.0040395*pow(P, 2) - 9.0361e-6*pow(P, 3)) :
               (-0.061108 - 0.0041626*P + 0.0040395*pow(P, 2) - 9.0361e-6*pow(P, 3));
    double y = ((-0.016427 - 0.011377*P + 0.0026284*pow(P, 2) - 5.8564e-6*pow(P, 3)) < 0) ?
               (-0.011377*P + 0.0026284*pow(P, 2) - 5.8564e-6*pow(P, 3)) :
               (-0.016427 - 0.011377*P + 0.0026284*pow(P, 2) - 5.8564e-6*pow(P, 3));
    double z = (fsand < 0.5) ? x : y;
    Q = (z > 0) ? z : 0;
}

} //namespace

