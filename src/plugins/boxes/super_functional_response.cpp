/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "super_functional_response.h"
using namespace base;

namespace boxes {

PUBLISH(SuperFunctionalResponse)

SuperFunctionalResponse::SuperFunctionalResponse(QString name, Box *parent)
    : Box(name, parent) {
    help("computes a Frazer-Gilbert functional response");
    Input(attacker).help("Attacker density").unit("#attackers");
    Input(host).help("Resource density").unit("#hosts");
    Input(demand).help("Number of attacks demanded").unit("#attacks");
    Input(attackRate).help("#attacks per #host per #attacker per time unit");
    Input(timeStep).equals(1).help("Time step").unit("Time unit");
    Output(supply).help("Number of attacks accomplished").unit("#attacks");
    Output(hostsAttacked).help("The number of hosts attacked").unit("#hosts");
    Output(propHostsAttacked).help("Proportion of hosts attacked").unit("[0;1]");
    Output(propAttackersAttacked).help("Proportion of successful attackers").unit("[0;1]");
    Output(sdRatio).help("Attackers's supply/demand ratio").unit("[0;1]");
}

void SuperFunctionalResponse::reset() {
    update();
}

void SuperFunctionalResponse::update() {
    double s = attackRate*attacker*timeStep;
    supply = (demand == 0.) ? 0. :
               demand*(1. - exp(-s*host/demand));

    double avgAttacksPerHost    = (host == 0.) ? 0. : supply/host,
          avgAttacksPerAttacker = (attacker == 0.) ? 0. : supply/attacker,
          propHostsNotAttacked = exp(-avgAttacksPerHost),
          propAttackersNotAttacked = exp(-avgAttacksPerAttacker);
    propHostsAttacked = 1. - propHostsNotAttacked;
    hostsAttacked = propHostsAttacked*host;
    sdRatio = (supply == 0.) ? 0. : supply/demand;
    propAttackersAttacked = 1. - propAttackersNotAttacked;
}

} //namespace

