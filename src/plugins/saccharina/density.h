/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DENSITY_H
#define DENSITY_H
#include <base/box.h>

namespace saccharina {

class Density : public base::Box
{
public:
    Density(QString name, Box *parent);
protected:
    // Outputs
    double dw, C, N, P;
};

}

#endif
