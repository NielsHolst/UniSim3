/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PHOTOSYNTHESIS_H
#define PHOTOSYNTHESIS_H
#include <base/box.h>

namespace saccharina {

class Photosynthesis : public base::Box
{
public:
    Photosynthesis(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double area, frondAngle, Iabsorbed,
        demand, alpha, fTemp, fNitrogen, timeStepSecs,
        Pmax;
    // Outputs
    double supply, sdRatio, P, Punlimited;
};

}

#endif
