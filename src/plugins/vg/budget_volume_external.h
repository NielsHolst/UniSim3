/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BUDGET_VOLUME_EXTERNAL_H
#define BUDGET_VOLUME_EXTERNAL_H
#include "budget_volume.h"

namespace vg {

class BudgetVolumeExternal : public BudgetVolume {
public:
    BudgetVolumeExternal(QString name, base::Box *parent);
    void update();
private:
    // Friends
    friend class Budget;
};

}

#endif
