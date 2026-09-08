/* Copyright 1998-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** William Meikle, USDA [william.meikle at usda.gov].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef EXPONENTIAL_PEAK_H
#define EXPONENTIAL_PEAK_H
#include <base/box.h>
#include <base/base_dllexport.h>

namespace cmbp {

class UNI_API ExponentialPeak : public base::Box
{
public:
    ExponentialPeak(QString name, Box *parent);
    void reset() override;
    void update() override;
private:
    // Input
    double optimum, d1, d2, x;
    // Output
    double value;
};

}

#endif
