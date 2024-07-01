/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef PRIMARY_DISTRIBUTION_BASE_H
#define PRIMARY_DISTRIBUTION_BASE_H

#include <base/box.h>

namespace PestTox {

class PrimaryDistributionBase : public base::Box
{
public:
    PrimaryDistributionBase(QString name, Box *parent);
protected:
    // Input
    double fieldArea, doseApplied;

    // Output
    double doseTaken, fractionTaken;
};

} //namespace


#endif
