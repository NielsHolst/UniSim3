/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RESERVES_H
#define RESERVES_H
#include <base/box.h>

namespace saccharina {

class Reserves : public base::Box
{
public:
    Reserves(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double initialProportion, maxProportion, allocatedProportion;
    // Outputs
    double proportion;
};

}

#endif
