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
    void reset();
    void update();
protected:
    // Inputs
    double
        swReflectivityTop, swReflectivityBottom,
        swTransmissivityTop, swTransmissivityBottom,
        lwReflectivityTop, lwReflectivityBottom,
        lwTransmissivityTop, lwTransmissivityBottom,
        Utop, Ubottom,
        heatCapacity;
    // Outputs
    double
        swAbsorptivityTop, swAbsorptivityBottom,
        lwAbsorptivityTop, lwAbsorptivityBottom;
protected:
    // Methods
    void updateAbsorptivities();
    void checkRange(double x, QString name) const;
};

} //namespace


#endif
