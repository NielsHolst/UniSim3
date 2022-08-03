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
    Area(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dryWeight, structuralMass, plantDensity, frondAngle, l, a, b;
    // Outputs
    double value, length, lai, crownZoneArea, kA;
    // Methods
    void updateByWeight(double w);
};

}

#endif
