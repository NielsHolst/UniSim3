/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SCALING_NITROGEN_H
#define SCALING_NITROGEN_H
#include <base/box.h>

namespace saccharina {

class ScalingNitrogen : public base::Box
{
public:
    ScalingNitrogen(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double concNmin, concNopt, concN;
    // Outputs
    double value;
};

}

#endif
