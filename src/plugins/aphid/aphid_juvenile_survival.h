/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef APHID_JUVENILE_SURVIVAL_H
#define APHID_JUVENILE_SURVIVAL_H
#include <base/box.h>

namespace aphid {

class AphidJuvenileSurvival : public base::Box
{
public:
    AphidJuvenileSurvival(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double cropGrowthStage, temperature;
    // Outputs
    double value;
};

}

#endif
