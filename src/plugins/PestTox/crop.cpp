/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include "crop.h"
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH(Crop)
	
Crop::Crop(QString name, Box *parent)
	: Box(name, parent)
{
    help("manages the fraction of seeds sown");
    Input(sowingDate).equals("1/5").help("Date of sowing");
    Input(date).imports("calendar[date]");

    Output(sown).unit("fraction").help("Seeds sown today ? (0 or 1)");
}

void Crop::update() {
    sown = (BareDate(date) == sowingDate) ? 1 : 0;
}

} //namespace

