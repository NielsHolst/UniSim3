/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include <base/any_year.h>
#include <base/publish.h>
#include "energy_budget.h"

using namespace base;

namespace apis {

PUBLISH(EnergyBudget)

EnergyBudget::EnergyBudget(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(supplyPotential).help("Potential supply of honey over this time step (g)");
    Input(demandRespiration).help("Respiration demand over this time step (g)");
    Input(demandGrowth).help("Growth demand over this time step, including conversion cost (g)");
    Input(costGrowth).imports("demand/growth[cost]");
    Input(storeHolding).help("Honey store holding (kg)");
    Input(storeCapacity).help("Maximum holding capacity (kg)");
    Output(supplyAvailable).help("Supply of honey available over this time step (g)");;
    Output(supplyRespiration).help("Respiration supply over this time step (g)");;
    Output(supplyGrowth).help("Growth supply over this time step, including conversion cost (g)");
    Output(changeInStore).help("Change in store holding over this time step (kg)");
    Output(supplyDemandRespiration).help("Supply/Demand ratio for respiration [0;1]");
    Output(supplyDemandGrowth).help("Supply/Demand ratio for respiration [0;1]");
}

void EnergyBudget::reset() {
}

void EnergyBudget::update() {
    // Set available supply within store margin
    double storageSpace =  1000*(storeCapacity-storeHolding) + demandRespiration + demandGrowth;
    supplyAvailable = std::min(supplyPotential, storageSpace);
    // Compute the ressource avaible for allocation
    double available = 1000*storeHolding + supplyAvailable;
    // Allocate to respiration
    if (available >= demandRespiration) {
        available -= supplyRespiration = demandRespiration;
        // Allocate to growth
        if (available >= demandGrowth)
            available -= supplyGrowth = demandGrowth;
        else {
            supplyGrowth = available;
            available = 0;
        }
    }
    else {
        supplyRespiration = available;
        available = supplyGrowth = 0;
    }
    // Change in store holding
    changeInStore = (supplyAvailable - supplyRespiration - supplyGrowth)/1000.;
    // Supply:demand ratios
    supplyDemandRespiration = (demandRespiration > 0.) ? supplyRespiration/demandRespiration : 0.;
    supplyDemandGrowth = (demandGrowth > 0.) ? supplyGrowth/demandGrowth : 0.;
}

}
