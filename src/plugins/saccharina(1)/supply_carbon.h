/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SUPPLY_CARBON_H
#define SUPPLY_CARBON_H
#include <base/box.h>

namespace saccharina {

class SupplyCarbon : public base::Box
{
public:
    SupplyCarbon(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double
        concN,
        k, lai, crownZoneArea,
        a, b, calib,
        E0;
    QVector<double> demands;
    // Outputs
    double Iabs, efficiency, demand, value;
};

}

#endif
