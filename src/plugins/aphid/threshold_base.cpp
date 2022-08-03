/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "threshold_base.h"

using namespace base;

namespace aphid {

ThresholdBase::ThresholdBase(QString name, Box *parent)
    : Box(name, parent)
{
    Input(cropGrowthStage).help("Crop growth stage").unit("Zadoks");
    Input(aphids).equals(0).help("Density of aphids").unit("per tiller");
    Output(exceeded).help("Was the threshold exceeded?").unit("boolean");
    Output(exceededAtGS).help("Crop growth when threshold was exceeded, or else 0").unit("Zadoks");
}

void ThresholdBase::reset() {
    exceeded = false;
    exceededAtGS = 0;
}

}
