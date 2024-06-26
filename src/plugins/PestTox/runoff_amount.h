/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef RUNOFF_AMOUNT_H
#define RUNOFF_AMOUNT_H

#include <base/box.h>

namespace PestTox {

class RunoffAmount : public base::Box
{
public:
    RunoffAmount(QString name, Box *parent);
    void update();

private:
    // Input
    double P, fsand;

    // Output
    double Q;
};

} //namespace


#endif
