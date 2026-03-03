/* Copyright 2018-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef YIELD_H
#define YIELD_H
#include <base/box.h>

namespace aphid {

class Yield : public base::Box
{
public:
    Yield(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double cropGrowthStage, aphidIndex;
    // Outputs
    double yield, loss, lossRate;
};

}

#endif
