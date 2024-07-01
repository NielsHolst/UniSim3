/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef LAYER_H
#define LAYER_H
#include <base/box.h>
#include "layer_parameters.h"

namespace vg {

class Layer : public base::Box, public LayerParameters
{
public:
    Layer(QString name, Box *parent);
private:
    // Inputs
    bool checkBoundaries;
protected:
    // Methods
    void useLayerAsInput();
    void useLayerAsOutput();
    void makeTransparent();
    void checkParameters() const;
private:
    void checkParameterBoundaries() const;
    void checkParameter(QString name, double value) const;
    void checkParameterSum(QString name, double value) const;
    void checkAbsorptivity(QString name, double value) const;
    void checkReflectivity(QString name, double value) const;
    // Friends
    friend class Budget;
    friend class BudgetLayer;
};

} //namespace


#endif
