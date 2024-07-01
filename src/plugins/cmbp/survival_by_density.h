/* Copyright 1998-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** William Meikle, USDA [william.meikle at usda.gov].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SURVIVAL_BY_DENSITY_H
#define SURVIVAL_BY_DENSITY_H
#include <base/box.h>

namespace cmbp {

class SurvivalByDensity : public base::Box
{
public:
    SurvivalByDensity(QString name, Box *parent);
    void reset();
    void update();
private:
    // Input
    double e1, N;
    // Output
    double value;
};

}

#endif
