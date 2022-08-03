/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LIGHT_ABSORPTION_LEAF_H
#define LIGHT_ABSORPTION_LEAF_H
#include <base/box.h>

namespace saccharina {

class LightAbsorptionLeaf : public base::Box
{
public:
    LightAbsorptionLeaf(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double I, area;
    // Outputs
    double value;
};

}

#endif
