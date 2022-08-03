/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SUPER_FUNCTIONAL_RESPONSE
#define SUPER_FUNCTIONAL_RESPONSE
#include <base/box.h>

namespace boxes {

class SuperFunctionalResponse : public base::Box
{
public: 
    SuperFunctionalResponse(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double demand, attacker, host, attackRate, timeStep;
    // Outputs
    double supply, propAttackersAttacked, propHostsAttacked,
      hostsAttacked, sdRatio;
};

} //namespace
#endif
