/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef THRESHOLD_BASE_H
#define THRESHOLD_BASE_H
#include <base/box.h>

namespace aphid {

class ThresholdBase : public base::Box
{
public:
    ThresholdBase(QString name, Box *parent);
    void reset();
protected:
    // Inputs
    double cropGrowthStage, aphids;
    // Outputs
    bool exceeded;
    double exceededAtGS;

};

}

#endif
