/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SUPPLY_NITROGEN_H
#define SUPPLY_NITROGEN_H
#include <base/box.h>

namespace saccharina {

class SupplyNitrogen : public base::Box
{
public:
    SupplyNitrogen(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double uptakeRate, area, N, timeStep;
    QVector<double> demands;
    // Outputs
    double demand, value;
};

}

#endif
