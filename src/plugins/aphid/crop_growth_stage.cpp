/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "crop_growth_stage.h"

using namespace base;

namespace aphid {

PUBLISH(CropGrowthStage)

CropGrowthStage::CropGrowthStage(QString name, Box *parent)
    : Box(name, parent)
{
    help("models wheat development based on Norwegian data");
    Input(valueAtStart).equals(20).help("Growth stage at the beginning of the growth season").unit("Zadoks");
    Input(dayDegrees).help("Day-degrees passed since growth season started").unit("DD");
    Input(dayDegreesHalfways).equals(720).help("Time when development is half completed").unit("DD");
    Input(slope).equals(2.8).help("Max. development rate").unit("DD-1");
    Output(value).help("Crop growth stage").unit("Zadoks");;
}

void CropGrowthStage::reset() {
    value = valueAtStart;
}

void CropGrowthStage::update() {
    const double maxGrowthStage = 99;
    value = (dayDegrees == 0.) ? valueAtStart :
             valueAtStart + (maxGrowthStage-valueAtStart)/(1.+exp(-slope*(log(dayDegrees)-log(dayDegreesHalfways))));
}

}
