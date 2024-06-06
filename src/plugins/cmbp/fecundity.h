/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** William Meikle, USDA [william.meikle at usda.gov].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FECUNDITY_H
#define FECUNDITY_H
#include <base/box.h>

namespace cmbp {

class Fecundity : public base::Box
{
public:
    Fecundity(QString name, Box *parent);
    void reset();
    void update();
private:
    // Input
    double a, b, c, T, sexRatio, calibration;
    // Output
    double value;
    // Methods
    double f(double T) const;
};

}

#endif
