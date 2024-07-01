/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SUPPLY_BUDGET
#define SUPPLY_BUDGET
#include <base/box.h>

namespace boxes {

class SupplyBudget : public base::Box
{
public: 
    SupplyBudget(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double supplyGross, demandNet, demandResp, egested, conversionCost;
    // Outputs
    double supplyNet, egestedAmount, conversionCostAmount, sdRatioNet;
};

} //namespace
#endif
