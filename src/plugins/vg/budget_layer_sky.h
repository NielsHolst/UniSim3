/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BUDGET_LAYER_SKY_H
#define BUDGET_LAYER_SKY_H
#include "budget_layer.h"

namespace vg {

class BudgetLayerSky : public BudgetLayer {
public:
    BudgetLayerSky(QString name, base::Box *parent);
    void update();
    void updateLwEmission();
    // Outputs
    double swCoverSkyBalance;
    // Friends
    friend class Budget;
};

}

#endif
