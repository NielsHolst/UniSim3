/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PRIMARY_DISTRIBUTION_SOIL_H
#define PRIMARY_DISTRIBUTION_SOIL_H

#include "primary_distribution_base.h"

namespace PestTox {

class PrimaryDistributionSoil : public PrimaryDistributionBase
{
public:
    PrimaryDistributionSoil(QString name, Box *parent);
    void update();

private:
    // Input
    double fractionTakenSurroundings, fractionTakenCrop;
};

} //namespace


#endif
