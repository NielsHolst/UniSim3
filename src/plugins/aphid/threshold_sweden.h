/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef THRESHOLD_SWEDEN_H
#define THRESHOLD_SWEDEN_H
#include "threshold_base.h"

namespace aphid {

class ThresholdSweden : public ThresholdBase
{
public:
    ThresholdSweden(QString name, Box *parent);
    void update();
};

}

#endif
