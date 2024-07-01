/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TOPSOIL_DEGRADATION_H
#define TOPSOIL_DEGRADATION_H

#include <base/box.h>

namespace PestTox {

class TopsoilDegradation : public base::Box
{
public:
    TopsoilDegradation(QString name, Box *parent);
    void update();

private:
    // Input
    double inflow, DT50, Tsoil, Rainfall,
    Vrsmc, rf, fw, Sp, leafWashOff, ksv, frsa,
    frsw, fsr, fmp;

    // Output
    double concentration, outflow, degradation, volatilization,
    ksd, fsdT, Vsmc, fsm, runoff, macroporeFlow;
};

} //namespace


#endif
