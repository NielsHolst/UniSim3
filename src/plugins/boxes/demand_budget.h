/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DEMAND_BUDGET
#define DEMAND_BUDGET
#include <base/box.h>

namespace boxes {

class DemandBudget : public base::Box
{
public: 
    DemandBudget(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double demandNet, demandResp, egested, conversionCost;
    // Outputs
    double demandGross;
};

} //namespace
#endif
