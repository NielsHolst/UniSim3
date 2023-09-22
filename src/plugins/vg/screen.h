/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SCREEN_H
#define SCREEN_H
#include "layer.h"
#include "layer_adjusted.h"

namespace vg {

class Screen : public Layer
{
public:
    Screen(QString name, Box *parent);
private:
    // Additional inputs
    double
        Uinsulation, UinsulationEffectivity,
        energySaving;
};

} //namespace


#endif
