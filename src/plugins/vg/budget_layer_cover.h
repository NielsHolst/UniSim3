/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BUDGET_LAYER_COVER_H
#define BUDGET_LAYER_COVER_H
#include "budget_layer.h"

namespace vg {

class BudgetLayerCover : public BudgetLayer {
public:
    BudgetLayerCover(QString name, base::Box *parent);
    double updateCondensation();
    void updateLwEmission();
    double updateDeltaTByEvapoCondensation(double condensation);
private:
    // Input
    double coverPerGroundArea, indoorsRh;
    // Output
    double condInsideDeltaT, condOutsideDeltaT, lwCoverSkyBalance, swCoverSkyBalance;
    // Friends
    friend class Budget;
};

}

#endif
