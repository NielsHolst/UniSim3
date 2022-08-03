/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef MUSSEL_BED_STARFISH_SALINITY_MORTALITY_H
#define MUSSEL_BED_STARFISH_SALINITY_MORTALITY_H

#include <base/box.h>

namespace MusselBed {

class StarfishSalMortality : public base::Box
{
public:
    StarfishSalMortality(QString name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double Smax, Smin;

    // Outputs
    double value;

};

} //namespace


#endif
