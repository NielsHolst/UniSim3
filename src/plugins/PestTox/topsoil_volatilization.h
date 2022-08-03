/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef TOPSOIL_VOLATILIZATION_H
#define TOPSOIL_VOLATILIZATION_H

#include <base/box.h>

namespace PestTox {

class TopsoilVolatilization : public base::Box
{
public:
    TopsoilVolatilization(QString name, Box *parent);
    void update();

private:
    // Input
    double Tair, Pd,
    MV, BLair, fa, fw, farea, Pair,
    Za, Zw, VZ, MW, viscosity;

    // Output
    double Ba, Dw, Dv, Dbl, Bea, Bew, Da, ksv,
    Bw, Dpl, ke;
};

} //namespace


#endif
