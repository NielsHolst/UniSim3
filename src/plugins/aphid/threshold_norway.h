/* Copyright 2018-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef THRESHOLD_NORWAY_H
#define THRESHOLD_NORWAY_H
#include "threshold_base.h"

namespace aphid {

class ThresholdNorway : public ThresholdBase
{
public:
    ThresholdNorway(QString name, Box *parent);
    void update();
};

}

#endif
