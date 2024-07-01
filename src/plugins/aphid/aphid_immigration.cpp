/* Copyright 2018-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "aphid_immigration.h"

//#include <base/dialog.h>

using namespace base;

namespace aphid {

PUBLISH(AphidImmigration)

AphidImmigration::AphidImmigration(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes daily immigration");
    Input(cropGrowthStage).help("Current crop growth stage").unit("Zadoks");
    Input(fromCropGrowthStage).help("Immigration begins as this growth stage").equals(31).unit("Zadoks");
    Input(toCropGrowthStage).help("Immigration ends as this growth stage").equals(80).unit("Zadoks");
    Input(immigrationRate).help("Immigration rate in the season").unit("per tiller d-1");
    Input(propExposedImmigrants).help("Proportion of infected immigrants").unit("[0;1]");
    Input(k).help("Has to match `k` of the receiving `StageAndPhase` box").unit("positive int");
    Output(total).help("Total immigration rate").unit("per tiller d-1");
    Output(susceptible).help("Immigration rate of susceptible aphids").unit("per tiller d-1");
    Output(exposed).help("Immigration rate of exposed aphids").unit("per tiller d-1");
}

void AphidImmigration::reset() {
    exposed.resize(k);
//    bool withoutFungus = findOne<Box*>("..")->name().contains("withoutFungus");
//    if (withoutFungus)
//        port("propExposedImmigrants")->equals(0.);
}

void AphidImmigration::update() {
//    auto prop = port("propExposedImmigrants");
//    QString original = prop->expression().originalAsString(),
//            stack    = prop->expression().stackAsString();
//    dialog().information(original);
//    dialog().information(stack);
//    prop->evaluate();
    total = (cropGrowthStage>fromCropGrowthStage && cropGrowthStage<toCropGrowthStage) ? immigrationRate : 0.;
    susceptible = total*(1.-propExposedImmigrants);
    // Number followed by all zeros in the rest of the vector
    exposed[0] = total*propExposedImmigrants;
}

}
