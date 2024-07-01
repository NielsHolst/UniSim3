/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef PRIMARY_DISTRIBUTION_AIR_H
#define PRIMARY_DISTRIBUTION_AIR_H

#include "primary_distribution_base.h"

namespace PestTox {

class PrimaryDistributionAir : public PrimaryDistributionBase
{
public:
    PrimaryDistributionAir(QString name, Box *parent);
    void update();

private:
    // Input
    double a1, b1, a2, b2, x;
};

} //namespace


#endif
