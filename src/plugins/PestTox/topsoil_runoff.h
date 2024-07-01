/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TOPSOIL_RUNOFF_H
#define TOPSOIL_RUNOFF_H

#include <base/box.h>

namespace PestTox {

class TopsoilRunoff : public base::Box
{
public:
    TopsoilRunoff(QString name, Box *parent);
    void update();

private:
    // Input
    double S, wbz, P, KocpH, fom, Q;

    // Output
    double Fslope, Fbuffer, F, fsr, Kd;
};

} //namespace


#endif
