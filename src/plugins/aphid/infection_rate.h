/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef INFECTION_RATE_H
#define INFECTION_RATE_H
#include <base/box.h>

namespace aphid {

class InfectionRate: public base::Box
{
public:
    InfectionRate(QString name, Box *parent);
    void update();
private:
    // Inputs
    bool isSporulating;
    double cadavers, transmissionEfficiency;
    // Outputs
    double value;

};

}

#endif
