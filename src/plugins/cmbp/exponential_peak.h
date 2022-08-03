/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** William Meikle, USDA [william.meikle at usda.gov].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef EXPONENTIAL_PEAK_H
#define EXPONENTIAL_PEAK_H
#include <base/box.h>

namespace cmbp {

class ExponentialPeak : public base::Box
{
public:
    ExponentialPeak(QString name, Box *parent);
    void reset();
    void update();
private:
    // Input
    double optimum, d1, d2, x;
    // Output
    double value;
};

}

#endif
