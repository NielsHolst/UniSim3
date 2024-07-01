/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef ZVALUES_H
#define ZVALUES_H

#include <base/box.h>

namespace PestTox {

class ZValues : public base::Box
{
public:
    ZValues(QString name, Box *parent);
    void update();

private:
    // Input
    double MW, Pd,
    Psol, VP, foc, pb, fa, fw, fs, farea,
    KocpH, Tair, R, Sp;

    // Output
    double Za, Zw, Zs, VPTadap, Vs, Vw, Va, VZ;
};

} //namespace


#endif
