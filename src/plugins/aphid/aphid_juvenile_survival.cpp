/* Copyright 2018-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <cmath>
#include <base/publish.h>
#include "aphid_juvenile_survival.h"

using std::max;
using namespace base;

namespace aphid {

PUBLISH(AphidJuvenileSurvival)

AphidJuvenileSurvival::AphidJuvenileSurvival(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes juvenile survival");
    Input(cropGrowthStage).help("Crop growth stage (`G`))").unit("Zadoks");
    Input(temperature).help("Daily average temperature (`T`)").unit("oC");
    Output(value).help("Juvenile survival (`y`)").unit("[0;1] d-1");
}

void AphidJuvenileSurvival::reset() {
    update();
}

void AphidJuvenileSurvival::update() {
    if(cropGrowthStage < 73.)
        value = max(0.944 - 3.32e-10*exp(0.726*temperature), 0.);
    else if (cropGrowthStage < 80.)
        value = 0.45;
    else
        value = 0.;
}

}
