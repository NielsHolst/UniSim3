/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "biocontrol.h"

using namespace base;

namespace aphid {

PUBLISH(Biocontrol)

Biocontrol::Biocontrol(QString name, Box *parent)
    : Box(name, parent)
{
    help("estimates biocontrol effects");
    Input(aphidPressureWithoutF).help("Accumulated aphid pressure without fungus").unit("aphid-days");
    Input(aphidPressureWithF).help("Accumulated aphid pressure with fungus").unit("aphid-days");

    Input(yieldWithoutF).help("Relative yield without fungus").unit("[0;1]");
    Input(yieldWithF).help("Relative yield witt fungus").unit("[0;1]");

    Input(cropGrowthStage).help("Current crop growth stage").unit("Zadoks");
    Input(prevalence).help("Prevalence of exposed aphids").unit("%");
    Input(cadaverPrevalence).help("Prevalence of cadavers").unit("%");

    Output(aphidPressureDifference).help("Difference in aphid pressure caused by fungus").unit("aphid-days");
    Output(yieldImprovement).help("Improvement in yield when controlled").unit("%-points");

    Output(percentageCadaversGs43).help("Percentage cadavers at GS 43").unit("%");
    Output(percentageCadaversGs61).help("Percentage cadavers at GS 61").unit("%");
    Output(percentageCadaversGs73).help("Percentage cadavers at GS 73").unit("%");

    Output(maxCadaverPrevalence).help("Peak cadaver prevalence before GS80").unit("%");
    Output(maxCadaverPrevalenceGS).help("Crop growth stage at peak cadaver prevalence before GS80").unit("Zadoks");

    Output(maxPrevalence).help("Peak prevalence before GS80").unit("%");
    Output(maxPrevalenceGS).help("Crop growth stage at peak prevalence before GS80").unit("Zadoks");
}

void Biocontrol::update() {
    aphidPressureDifference = aphidPressureWithoutF - aphidPressureWithF;
    yieldImprovement = 100.*(yieldWithF - yieldWithoutF);

    if (cropGrowthStage > 43. && percentageCadaversGs43==0.)
        percentageCadaversGs43 = cadaverPrevalence;
    if (cropGrowthStage > 61. && percentageCadaversGs61==0.)
        percentageCadaversGs61 = cadaverPrevalence;
    if (cropGrowthStage > 73. && percentageCadaversGs73==0.)
        percentageCadaversGs73 = cadaverPrevalence;

    if (cropGrowthStage > 55. && cropGrowthStage < 80.) {
        if (prevalence > maxPrevalence) {
            maxPrevalence   = prevalence;
            maxPrevalenceGS = cropGrowthStage;
        }
        if (cadaverPrevalence > maxCadaverPrevalence) {
            maxCadaverPrevalence   = cadaverPrevalence;
            maxCadaverPrevalenceGS = cropGrowthStage;
        }
    }
}

}
