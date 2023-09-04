/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LAYER_H
#define LAYER_H
#include <base/box.h>

namespace vg {

class Layer : virtual public base::Box
{
public:
    Layer(QString name, Box *parent);
    void update();
    void updateInputsFromProduct(QString productPath);
protected:
    // Inputs
    double
        swAbsorptivityTop, swAbsorptivityBottom,
        swReflectivityTop, swReflectivityBottom,
        swTransmissivityTop, swTransmissivityBottom,
        lwAbsorptivityTop, lwAbsorptivityBottom,
        lwReflectivityTop, lwReflectivityBottom,
        lwTransmissivityTop, lwTransmissivityBottom,
        Utop, Ubottom,
        heatCapacity;
    // Methods
    void checkInputs();
    void checkInput(QString name, double value);
    void checkInputSum(QString name, double value);
};

} //namespace


#endif
