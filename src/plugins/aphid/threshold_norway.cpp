/* Copyright 2018-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "threshold_norway.h"

using namespace base;

namespace aphid {

PUBLISH(ThresholdNorway)

ThresholdNorway::ThresholdNorway(QString name, Box *parent)
    : ThresholdBase(name, parent)
{
    help("evaluates aphid damage threshold used in Norway (VIPS, Klingen 2008");
}

void ThresholdNorway::update() {
    // From VIPS Klingen et al. 2008
    const double &GS(cropGrowthStage); // Create a shorthand name
    if (!exceeded) { // to make sure exceededAtGS will not change
//        if (GS>41 && GS<49) // in Ingeborg's ppt
//            exceeded = (aphids > 3); // || perc_infested>60 but we cannot have that information in this model
//        else if (GS>68 && GS<70) // in Ingeborg's ppt is = 69 but C++ doesn't like this statement: it is "unsafe"
//            exceeded = (aphids > 10); // || perc_infested>90 but we cannot have that information in this model
//        else if (GS>73 && GS<82) // in Ingeborg's ppt
//            exceeded = (aphids > 15); // || perc_infested>95 but we cannot have that information in this model

        if (GS<41)
            exceeded = false;
        else if (GS<49)
            exceeded = (aphids > 3);
        else if (GS<69)
            exceeded = (aphids > 10);
        else if (GS<77) //82
            exceeded = (aphids > 15);
        else
            exceeded = false;
        if (exceeded)
            exceededAtGS = GS;
    }
}

}
