/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CARRYING_CAPACITY_H
#define CARRYING_CAPACITY_H
#include <base/box.h>

namespace resist {

class CarryingCapacity : public base::Box
{
public:
    CarryingCapacity(QString name, QObject *parent);
private:
    // Inputs
    double value;
};

}

#endif
