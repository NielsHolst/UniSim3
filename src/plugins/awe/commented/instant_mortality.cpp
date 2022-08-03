/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/any_year.h>
#include <base/publish.h>
#include "instant_mortality.h"

using namespace base;

namespace awe {

PUBLISH(InstantMortality)

InstantMortality::InstantMortality(QString, QObject *parent)
	: Model(name,parent) 
{ 
    Input(today).imports("calendar[date]");
    Input(date).equals("1/6/*").help("Date of event");
    Input(seedlingMortality);
    Input(juvenileMortality);
    Input(matureMortality);
    Input(seedsOnPlantMortality);
    Output(seedlingMortalityToday);
    Output(juvenileMortalityToday);
    Output(matureMortalityToday);
    Output(seedsOnPlantMortalityToday);
}

void InstantMortality::update() {
    if (equals(date, today)) {
        seedlingMortalityToday     = seedlingMortality;
        juvenileMortalityToday     = juvenileMortality;
        matureMortalityToday       = matureMortality;
        seedsOnPlantMortalityToday = seedsOnPlantMortality;
    }
    else {
        seedlingMortalityToday     = 0;
        juvenileMortalityToday     = 0;
        matureMortalityToday       = 0;
        seedsOnPlantMortalityToday = 0;
    }
}

}
