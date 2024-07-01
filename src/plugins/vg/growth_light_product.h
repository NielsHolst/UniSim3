/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef GROWTH_LIGHT_PRODUCT_H
#define GROWTH_LIGHT_PRODUCT_H

#include <base/box.h>

namespace vg {

class GrowthLightProduct : public base::Box {
public:
    GrowthLightProduct(QString name, base::Box *parent);
private:
    // Inputs
    double ballast, power,
        parPhotonCoef,
        propSw, propLw, propConv,
        propBallastLw, propBallastConv,
        minPeriodOn;
};
} //namespace


#endif
