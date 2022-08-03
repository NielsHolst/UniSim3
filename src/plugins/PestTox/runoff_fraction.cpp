/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "runoff_fraction.h"
#include <base/publish.h>


using namespace base;

namespace PestTox {

PUBLISH (RunoffFraction)
	
RunoffFraction::RunoffFraction(QString name, Box *parent)
	: Box(name, parent)
{
    help("manages runoff fraction");
    Input(Q).unit("mm").equals(5.).help("Runoff amount");
    Input(P).unit("mm").equals(1.).help("Daily rainfall");

    Output(frr).unit("fraction").help("Fraction of rain runoff");
}

void RunoffFraction::update() {
    frr = (P > 0) ? Q/P : 0;
}

} //namespace

