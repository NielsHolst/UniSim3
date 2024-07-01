/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef MACROPORE_FLOW_H
#define MACROPORE_FLOW_H

#include <base/box.h>

namespace PestTox {

class MacroporeFlow : public base::Box
{
public:
    MacroporeFlow(QString name, Box *parent);
    void update();

private:
    // Input
    double fsand, fsilt, fclay, fw, fa, fmacropore, Flowrate,
    frr, tpevent, P, frsw, Tf;

    // Output
    double fporemobile, Vporeimmobile, C, Cused, Cfree,
    Pcapacity, Preqh, Preqday, Pi,
    fmp, Vporemobile;
};

} //namespace


#endif
