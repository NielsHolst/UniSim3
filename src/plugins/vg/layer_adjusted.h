/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LAYER_ADJUSTED_H
#define LAYER_ADJUSTED_H
#include <base/box.h>

namespace vg {

class LayerAdjusted : virtual public base::Box
{
public:
    LayerAdjusted(QString name, Box *parent);
    void update();
protected:
    // Inputs
    double coverPerGroundArea;
    // Outputs
    double
        swReflectivityTopAdj, swReflectivityBottomAdj,
        swTransmissivityTopAdj, swTransmissivityBottomAdj,
        lwReflectivityTopAdj, lwReflectivityBottomAdj,
        lwTransmissivityTopAdj, lwTransmissivityBottomAdj,
        swAbsorptivityTopAdj, swAbsorptivityBottomAdj,
        lwAbsorptivityTopAdj, lwAbsorptivityBottomAdj,
        UtopAdj, UbottomAdj,
        heatCapacityAdj;
    // Methods
    void makeTransparent();
    void checkOutputs();
    void checkOutput(QString name, double value);
    void checkOutputSum(QString name, double value);
    // Friends
    friend class Budget;
    friend class BudgetLayer;
};

} //namespace


#endif
