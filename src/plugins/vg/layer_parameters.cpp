/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include <base/phys_math.h>
#include "layer_parameters.h"

using namespace base;
using phys_math::infinity;

namespace vg {

LayerParameters::LayerParameters() {
}

void LayerParameters::makeTransparent() {
    swReflectivityTop   = swReflectivityBottom   = 0.;
    swTransmissivityTop = swTransmissivityBottom = 1.;
    lwReflectivityTop   = lwReflectivityBottom   = 0.;
    lwTransmissivityTop = lwTransmissivityBottom = 1.;
    swAbsorptivityTop   = swAbsorptivityBottom   = 0.;
    lwAbsorptivityTop   = lwAbsorptivityBottom   = 0.;
    Utop = Ubottom = infinity();
    heatCapacity = 0.;
}

#define SET_POINTER(x) x = box->port(#x)->valuePtr<double>()

void LayerParametersPtrs::setPointers(Box *box) {
    SET_POINTER(swReflectivityTop);
    SET_POINTER(swReflectivityTop);
    SET_POINTER(swReflectivityBottom);
    SET_POINTER(swTransmissivityTop);
    SET_POINTER(swTransmissivityBottom);
    SET_POINTER(lwReflectivityTop);
    SET_POINTER(lwReflectivityBottom);
    SET_POINTER(lwTransmissivityTop);
    SET_POINTER(lwTransmissivityBottom);
    SET_POINTER(swAbsorptivityTop);
    SET_POINTER(swAbsorptivityBottom);
    SET_POINTER(lwAbsorptivityTop);
    SET_POINTER(lwAbsorptivityBottom);
    SET_POINTER(Utop);
    SET_POINTER(Ubottom);
    SET_POINTER(heatCapacity);
}

} //namespace

