/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef APHID_OFFSPRING_H
#define APHID_OFFSPRING_H
#include <base/box.h>

namespace aphid {

class AphidOffspring : public base::Box
{
public:
    AphidOffspring(QString name, Box *parent);
    void update();
private:
    // Inputs
    double offspringFromSusceptible,
        offspringFromExposedApterous, offspringFromExposedAlate,
        aphidDensity, cropGrowthStage;
    // Outputs
    double alate, apterous, alateProportion;
};

}

#endif
