/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef AREA_H
#define AREA_H
#include <base/box.h>

namespace saccharina {

class Area : public base::Box
{
public:
    Area(QString name, Box *parent);
    void reset();
    void update();
protected:
    // Inputs
    double ca, ea, cw, ew, cz, biomass;
    // Outputs
    double leaf, crownZoneWidth, crownZone, index;
};

}

#endif
