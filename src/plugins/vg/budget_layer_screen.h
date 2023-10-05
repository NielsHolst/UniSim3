/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BUDGET_LAYER_SCREEN_H
#define BUDGET_LAYER_SCREEN_H
#include "budget_layer.h"

namespace vg {

class BudgetLayerScreen : public BudgetLayer {
public:
    BudgetLayerScreen(QString name, base::Box *parent);
    double updateCondensation();
    void updateLwEmission();
private:
    double groundArea, indoorsRh;
    // Friends
    friend class Budget;
};

}

#endif
