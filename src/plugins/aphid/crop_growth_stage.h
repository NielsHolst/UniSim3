/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CROP_GROWTH_STAGE_H
#define CROP_GROWTH_STAGE_H
#include <base/box.h>

namespace aphid {

class CropGrowthStage : public base::Box
{
public:
    CropGrowthStage(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double dayDegrees, valueAtStart, dayDegreesHalfways, slope;
    // Outputs
    double value;
};

}

#endif
