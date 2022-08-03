/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef NITROGEN_UPTAKE_H
#define NITROGEN_UPTAKE_H
#include <base/box.h>

namespace saccharina {

class NitrogenUptake : public base::Box
{
public:
    NitrogenUptake(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double beta, demand, area, fCurrent, N, timeStepSecs, dryWeight;
    // Outputs
    double supply, sdRatio, J, V, B;
};

}

#endif
