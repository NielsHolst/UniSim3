/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef APHID_NET_REPRODUCTION_H
#define APHID_NET_REPRODUCTION_H
#include <base/box.h>

namespace aphid {

class AphidNetReproduction : public base::Box
{
public:
    AphidNetReproduction(QString name, Box *parent);
    void update();
private:
    // Inputs
    double Tmin, Tmax, Topt, R0opt, temperature,
        cropGrowthStage,
        optimumCropGrowthStageFrom, optimumCropGrowthStageTo, optimumCropFactor,
        alateFactor,
        aphidDensity, aphidDensityThreshold, exposureCost;
    // Outputs
    double apterous, alate,
        apterousExposed, alateExposed;
};

}

#endif
