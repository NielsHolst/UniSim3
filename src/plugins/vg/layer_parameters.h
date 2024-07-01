/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef LAYER_PARAMETERS_H
#define LAYER_PARAMETERS_H

namespace base {
    class Box;
}

namespace vg {

struct LayerParameters
{
    LayerParameters();
    // Parameters serving as inputs or outputs
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
    void makeTransparent();
};

struct LayerParametersPtrs {
    LayerParametersPtrs() {}
    void setPointers(base::Box *box);
    const double
        *swReflectivityTop,   *swReflectivityBottom,
        *swTransmissivityTop, *swTransmissivityBottom,
        *lwReflectivityTop,   *lwReflectivityBottom,
        *lwTransmissivityTop, *lwTransmissivityBottom,
        *swAbsorptivityTop,   *swAbsorptivityBottom,
        *lwAbsorptivityTop,   *lwAbsorptivityBottom,
        *Utop,                *Ubottom,
        *heatCapacity;
};

} //namespace


#endif
