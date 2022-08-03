/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ENERGY_BUDGET_H
#define ENERGY_BUDGET_H
#include <base/box.h>

namespace apis {

class EnergyBudget : public base::Box
{
public:
    EnergyBudget(QString name, QObject *parent);
    void reset();
    void update();
private:
    // input
    double supplyPotential, demandRespiration, demandGrowth, costGrowth,
        storeHolding, storeCapacity;
    // output
    double supplyAvailable, supplyRespiration, supplyGrowth, changeInStore,
        supplyDemandRespiration, supplyDemandGrowth;
};

}

#endif
