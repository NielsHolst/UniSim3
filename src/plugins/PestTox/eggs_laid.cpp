/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include "eggs_laid.h"
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (EggsLaid)
	
EggsLaid::EggsLaid(QString name, Box *parent)
	: Box(name, parent)
{
    help("manages the number of eggs laid");
    Input(eggsPerFemale).equals(100.).help("number of eggs laid per female");
    Input(population).equals(0.).help("current population");
    Input(sexRatio).unit("fraction").equals(0.5).help("fraction of females in the population");

    Output(totalEggs);
}

void EggsLaid::update() {
    totalEggs = sexRatio*eggsPerFemale;
}

} //namespace

