/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef FUNCTIONAL_RESPONSE
#define FUNCTIONAL_RESPONSE

#include <base/box.h>
#include <base/base_dllexport.h>

namespace boxes {

class UNI_API FunctionalResponse : public base::Box
{
public: 
    FunctionalResponse(QString name, Box *parent);
    void reset() override;
    void update() override;
private:
    // Inputs
    double attacker, prey, attackRate, demandGross, timeStep;
    // Outputs
    double supplyGross, propPreyAttacked, searchEfficacy, sdRatioGross;
};

} //namespace
#endif
