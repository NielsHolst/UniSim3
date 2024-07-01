/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef ORGANIC_CARBON_WATER_PARTITIONING_H
#define ORGANIC_CARBON_WATER_PARTITIONING_H

#include <base/box.h>

namespace PestTox {

class OrganicCarbonWaterPartitioning : public base::Box
{
public:
    OrganicCarbonWaterPartitioning(QString name, Box *parent);
    void update();

private:
    // Input
    double Koc, pH, pKa;

    // Output
    double KocpH;
};

} //namespace


#endif
