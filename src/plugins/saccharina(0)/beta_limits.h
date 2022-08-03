/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BETA_LIMITS
#define BETA_LIMITS
#include <base/box.h>

namespace saccharina {

class BetaLimits : public base::Box
{
public:
    BetaLimits(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double
        g, kg, gSlopeFloor, gInterceptFloor, gSlopeCeiling, gInterceptCeiling,
        Pmax, PmaxSlope, PmaxIntercept;
    // Outputs
    double min, max;
};

}

#endif
