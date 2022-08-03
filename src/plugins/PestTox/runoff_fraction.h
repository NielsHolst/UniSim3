/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RUNOFF_FRACTION_H
#define RUNOFF_FRACTION_H

#include <base/box.h>

namespace PestTox {

class RunoffFraction : public base::Box
{
public:
    RunoffFraction(QString name, Box *parent);
    void update();

private:
    // Input
    double Q, P;

    // Output
    double frr;
};

} //namespace


#endif
