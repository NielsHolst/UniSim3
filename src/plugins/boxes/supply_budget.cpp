/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "supply_budget.h"
using namespace base;

namespace boxes {

PUBLISH(SupplyBudget)

SupplyBudget::SupplyBudget(QString name, Box *parent)
    : Box(name, parent) {
    help("computes net supply from energy budget");
    Input(supplyGross).help("Gross supply").unit("#prey");
    Input(demandNet).help("Net demand").unit("#prey");
    Input(demandResp).help("Respiration demand").unit("#prey");
    Input(egested).help("Egested or unused fraction of prey").unit("[0;1]");
    Input(conversionCost).help("Cost of converting prey into consumer)").unit("[0;1]");
    Output(supplyNet).help("Net supply for predator assimilation").unit("#prey");
    Output(egestedAmount).help("Egested (or unused) amount of resource").unit("#prey");
    Output(conversionCostAmount).help("Cost of converting resource into consumer").unit("#prey");
    Output(sdRatioNet).help("Predator's net supply/demand ratio").unit("[0;1]");
}

void SupplyBudget::reset() {
    update();
}

void SupplyBudget::update() {
    egestedAmount = egested*supplyGross;
    conversionCostAmount = conversionCost*(supplyGross - egestedAmount - demandResp);
    supplyNet =  supplyGross - egestedAmount - conversionCostAmount - demandResp;
    sdRatioNet = (demandNet == 0.) ? 0. : supplyNet/demandNet;
}

} //namespace

