/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef WATER_BUDGET_H
#define WATER_BUDGET_H
#include <base/box.h>

namespace coffee {

class WaterBudget : public base::Box
{
public: 
    WaterBudget(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double runoffLaiCoef,
        concWaterFieldCap, rootDepth, timeStep, slope,
        rainfall, rainIntercepted,
        laiCoffee, laiTree,
        evaporationCoffee,
        transpirationCoffee, transpirationTree;
    // Outputs
    double runoff, drain, availableWater;
    // Data
    double _availableAtFieldCap;
};

} //namespace
#endif
