/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FUNCTIONAL_RESPONSE
#define FUNCTIONAL_RESPONSE
#include <base/box.h>

namespace boxes {

class FunctionalResponse : public base::Box
{
public: 
    FunctionalResponse(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double attacker, prey, attackRate, demandGross, timeStep;
    // Outputs
    double supplyGross, propPreyAttacked, searchEfficacy, sdRatioGross;
};

} //namespace
#endif
