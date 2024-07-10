/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SKY_H
#define SKY_H

#include "layer_as_output.h"

namespace vg {

class Sky : public LayerAsOutput
{
public:
    Sky(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double slope, intercept, airTemperature, rh, Tsky;
    // Output
    double temperature;
};
} //namespace


#endif
