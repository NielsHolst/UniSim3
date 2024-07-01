/* Copyright 2019-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Teis Boderskov, Aarhus University [tebo atbios.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef PLANT_COMPARTMENT_H
#define PLANT_COMPARTMENT_H
#include "density.h"

namespace saccharina {

class PlantCompartment : public Density
{
public:
    PlantCompartment(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double concC, concN, concP,
        dwInit, dwIncrement;
};

}

#endif
