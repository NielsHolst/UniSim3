/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef AREA_EROSION_H
#define AREA_EROSION_H
#include <base/box.h>

namespace saccharina {

class AreaErosion : public base::Box
{
public:
    AreaErosion(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double A, e50, eSlope, eMax, timeStepSecs;
    // Outputs
    double value;
};

}

#endif
