/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef NITROGEN_SUPPLY_H
#define NITROGEN_SUPPLY_H
#include <base/box.h>

namespace coffee {

class NitrogenSupply : public base::Box
{
public: 
    NitrogenSupply(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double rootC,
        efficiency, soilN50,
        soilNitrogen, timeStep;
    // Outputs
    double value;
};

} //namespace
#endif
