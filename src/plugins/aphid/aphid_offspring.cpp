/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <cmath>
#include <base/publish.h>
#include "aphid_offspring.h"

using namespace base;

namespace aphid {

PUBLISH(AphidOffspring)

AphidOffspring::AphidOffspring(QString name, Box *parent)
    : Box(name, parent)
{
    help("splits new offspring into apterous and alate morphs");
    Input(offspringFromSusceptible).help("Total no. of offspring produced by susceptible aphids").unit("per tiller");
    Input(offspringFromExposedApterous).help("Total no. of offspring produced by exposed apterous aphids").unit("per tiller");
    Input(offspringFromExposedAlate).help("Total no. of offspring produced by exposed alate aphids").unit("per tiller");
    Input(aphidDensity).help("Aphid density").unit("per tiller");
    Input(cropGrowthStage).help("Crop growth stage").unit("Zadoks");
    Output(apterous).help("Total no. of apterous offspring produced").unit("per tiller");
    Output(alate).help("Total no. of alate offspring produced").unit("per tiller");
    Output(alateProportion).help("Proportion of alate offspring").unit("[0;1]");
}

void AphidOffspring::update() {
    alateProportion = (2.603*aphidDensity + 0.847*cropGrowthStage - 27.189)/100.;
    if (alateProportion<0.)
        alateProportion=0.;
    else if (alateProportion>1.)
        alateProportion = 1.;
    double offspringTotal = offspringFromSusceptible + offspringFromExposedApterous + offspringFromExposedAlate;
    apterous = (1. - alateProportion)*offspringTotal;
    alate = alateProportion*offspringTotal;
}


}
