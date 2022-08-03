/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CARBON_RESERVES_MAX_H
#define CARBON_RESERVES_MAX_H
#include <base/box.h>

namespace saccharina {

class CarbonReservesMax : public base::Box
{
public:
    CarbonReservesMax(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double youngMax, matureMax, maturity;
    // Outputs
    double value;
};

}

#endif
