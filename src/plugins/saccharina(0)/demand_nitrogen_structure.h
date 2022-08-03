/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DEMAND_NITROGEN_STRUCTURE_H
#define DEMAND_NITROGEN_STRUCTURE_H
#include <base/box.h>

namespace saccharina {

class DemandNitrogenStructure : public base::Box
{
public:
    DemandNitrogenStructure(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double proportionN, proportionC, demandCarbonStructure;
    // Outputs
    double value;
};

}

#endif
