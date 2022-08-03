/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PMAX_MINIMUM
#define PMAX_MINIMUM
#include <base/box.h>

namespace saccharina {

class PmaxMinimum : public base::Box
{
public:
    PmaxMinimum(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double g, slope, intercept, floor;
    // Outputs
    double value;
};

}

#endif
