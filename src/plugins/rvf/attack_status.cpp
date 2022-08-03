/* Copyright 2012-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Clement N. Mweya, National Institute for Medical Research, Tukuyu, Tanzania [cmweya at nimr.or.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "attack_status.h"

using namespace base;

namespace rvf {
  
PUBLISH(AttackStatus)
  
AttackStatus::AttackStatus(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(infectiousAttacks).help("Number of infectious attacks");
    Input(allAttacks).help("Total number of attacks");
    Output(propInfectious).help("Proportion of infectious attacks").unit("[0;1]");
    Output(propNoninfectious).help("Proportion of non-infectious attacks").unit("[0;1]");
}

void AttackStatus::reset() {
    update();
}

void AttackStatus::update() {
    propInfectious = (allAttacks == 0.) ? 0. : infectiousAttacks/allAttacks;
    propNoninfectious = 1. - propInfectious;
}

} //namespace

