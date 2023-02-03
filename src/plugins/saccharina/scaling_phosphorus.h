/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SCALING_PHOSPHORUS_H
#define SCALING_PHOSPHORUS_H
#include <base/box.h>

namespace saccharina {

class ScalingPhosphorus : public base::Box
{
public:
    ScalingPhosphorus(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double concPopt, concP;
    // Outputs
    double value;
};

}

#endif
