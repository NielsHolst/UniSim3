/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "demand_budget.h"
using namespace base;

namespace boxes {

PUBLISH(DemandBudget)

DemandBudget::DemandBudget(QString name, Box *parent)
    : Box(name, parent) {
    help("computes gross demand from energy budget");
    Input(demandNet).help("Net demand").unit("#prey");
    Input(demandResp).help("Respiration demand").unit("#prey");
    Input(egested).help("Egested or unused fraction of prey").unit("[0;1]");
    Input(conversionCost).help("Cost of converting prey into consumer)").unit("[0;1]");
    Output(demandGross).help("Gross demand including all expenditures").unit("#prey");
}

void DemandBudget::reset() {
    update();
}

void DemandBudget::update() {
    demandGross = (demandNet/(1. - conversionCost) + demandResp)/(1. - egested);
}

} //namespace

