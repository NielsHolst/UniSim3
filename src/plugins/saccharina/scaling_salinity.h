/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SCALING_SALINITY_H
#define SCALING_SALINITY_H
#include <base/box.h>

namespace saccharina {

class ScalingSalinity : public base::Box
{
public:
    ScalingSalinity(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double S, Sopt, S50;
    // Outputs
    double value;
};

}

#endif
