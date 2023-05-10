/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FLOOR_H
#define FLOOR_H
#include "layer.h"
#include "layer_adjusted.h"

namespace vg {

class Floor : public Layer, public LayerAdjusted
{
public:
    Floor(QString name, Box *parent);
    void reset();
    void update();
};

} //namespace


#endif