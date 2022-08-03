/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "functional_response.h"
using namespace base;

namespace boxes {

PUBLISH(FunctionalResponse)

FunctionalResponse::FunctionalResponse(QString name, Box *parent)
    : Box(name, parent) {
    help("computes a Gutierrez-BaumgÃ¤rtner functional response");
    Input(attacker).help("Attacker density").unit("#predators");
    Input(prey).help("Resource density").unit("#prey");
    Input(demandGross).help("Gross demand of attacker").unit("#prey");
    Input(attackRate).help("#attacks per #prey per #predator per time unit");
    Input(timeStep).equals(1).help("Time step").unit("Time unit");
    Output(supplyGross).help("Gross supply (number of prey attacked)").unit("#prey");
    Output(propPreyAttacked).help("The proportion of prey attacked").unit("[0;1]");
    Output(searchEfficacy).help("Efficacy in finding prey [0;1]");
    Output(sdRatioGross).help("Predator's gross supply/demand ratio").unit("[0;1]");
}

void FunctionalResponse::reset() {
    update();
}

void FunctionalResponse::update() {
    searchEfficacy = 1. - exp(-attackRate*attacker*timeStep);
    supplyGross = (demandGross == 0.) ? 0. : demandGross*(1. - exp(-searchEfficacy*prey/demandGross));
    propPreyAttacked = (prey == 0.) ? 0. : supplyGross/prey;
    sdRatioGross = (demandGross == 0.) ? 0. : supplyGross/demandGross;
    // Guard against numerical inaccuracy
    if (supplyGross > demandGross)
        supplyGross = demandGross;
    if (supplyGross > prey)
        supplyGross = prey;
    if (propPreyAttacked > 1.)
        propPreyAttacked = 1.;
    if (sdRatioGross > 1.)
        sdRatioGross = 1.;
}

} //namespace

