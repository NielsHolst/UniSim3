/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FOOD_WEB_CALC_PREDATOR_PREY_H
#define FOOD_WEB_CALC_PREDATOR_PREY_H
#include "food_web_calc_base.h"

namespace boxes {

class FoodWebCalcPredatorPrey : public FoodWebCalcBase
{
public: 
    FoodWebCalcPredatorPrey(QString name, Box *parent);
    void amend();
    void reset();
protected:
    // Outputs (indirect)
    base::Matrix2D<double> _Xloss;
    QVector<double> _XlossTotal, _mortality;
    // Methods
    virtual void computeSearchRates();
    virtual void computeLosses();
    virtual QString attackerClass() const {return "Predator";}
    virtual QString resourceClass() const {return "Prey";}
};

} //namespace
#endif
