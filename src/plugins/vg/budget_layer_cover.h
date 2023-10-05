/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
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
    double updateDeltaTByCondensation(double insideCondensation, double outsideCondensation);
private:
    // Input
    double coverPerGroundArea, indoorsRh;
    // Output
    double condInsideDeltaT, condOutsideDeltaT;
    // Friends
    friend class Budget;
};

}

#endif
