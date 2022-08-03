/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef STARFISH_DEMAND_H
#define STARFISH_DEMAND_H

#include <base/box.h>

namespace MusselBed {

class StarfishDemand : public base::Box
{
public:
    StarfishDemand(QString name, QObject *parent);
    void update();

private:
    // Inputs
    double
        aGrowth, bGrowth, aResp, bResp,
        size, temperature, biomass;

    // Outputs
    double netDemand, respiration;

};

} //namespace


#endif
