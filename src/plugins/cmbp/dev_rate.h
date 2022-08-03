/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** William Meikle, USDA [william.meikle at usda.gov].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DEV_RATE_H
#define DEV_RATE_H
#include <base/box.h>

namespace cmbp {

class DevRate : public base::Box
{
public:
    DevRate(QString name, Box *parent);
    void reset();
    void update();
private:
    // Input
    double b1, b2, b3, b4, T;
    // Output
    double value;
};

}

#endif
