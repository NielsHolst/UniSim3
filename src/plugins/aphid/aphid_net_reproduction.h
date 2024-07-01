/* Copyright 2018-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
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
        optimumCropGrowthStageMin, optimumCropGrowthStageMax, optimumCropFactor,
        alateFactor,
        aphidDensity, aphidDensityThreshold, immunityCost;
    // Outputs
    double apterous, alate,
        apterousExposed, alateExposed;
};

}

#endif
