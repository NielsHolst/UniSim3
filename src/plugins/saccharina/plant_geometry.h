/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PLANT_GEOMETRY_H
#define PLANT_GEOMETRY_H
#include <base/box.h>

namespace saccharina {

class PlantGeometry : public base::Box
{
public:
    PlantGeometry(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double
        biomassStruct, areaCoef,
        biomassTotal, a, b, c, d,
        relWidth;
    // Outputs
    double area, length, crownZoneArea, lai;
};

}

#endif
