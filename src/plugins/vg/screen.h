/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SCREEN_H
#define SCREEN_H
#include "layer_as_input.h"

namespace vg {

class Screen : public LayerAsInput
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
