/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include "layer_as_input.h"

using namespace base;

namespace vg {

LayerAsInput::LayerAsInput(QString name, Box *parent)
    : Layer(name, parent)
{
    help("holds radiative and heat parameters of a layer used for parameter input");
    Input(swAbsorptivityTop).unit("[0;1]").help("Short-wave absorptivity at the top");
    Input(swReflectivityTop).unit("[0;1]").help("Short-wave reflectivity at the top");
    Input(swTransmissivityTop).equals(1.).unit("[0;1]").help("Short-wave transmissivity at the top");

    Input(swAbsorptivityBottom).unit("[0;1]").help("Short-wave absorptivity at the bottom");
    Input(swReflectivityBottom).unit("[0;1]").help("Short-wave reflectivity at the bottom");
    Input(swTransmissivityBottom).equals(1.).unit("[0;1]").help("Short-wave transmissivity at the bottom");

    Input(lwAbsorptivityTop).unit("[0;1]").help("Long-wave absorptivity at the top");
    Input(lwReflectivityTop).unit("[0;1]").help("Long-wave reflectivity at the top");
    Input(lwTransmissivityTop).equals(1.).unit("[0;1]").help("Long-wave transmissivity at the top");

    Input(lwAbsorptivityBottom).unit("[0;1]").help("Long-wave absorptivity at the bottom");
    Input(lwReflectivityBottom).unit("[0;1]").help("Long-wave reflectivity at the bottom");
    Input(lwTransmissivityBottom).equals(1.).unit("[0;1]").help("Long-wave transmissivity at the bottom");

    // When used as input these ports are per layer area
    Input(Utop).unit("W/K/m2 layer").help("Heat transfer coefficient at the top");
    Input(Ubottom).unit("W/K/m2 layer").help("Heat transfer coefficient at the bottom");
    Input(heatCapacity).help("Area-specific heat capacity").unit("J/K/m2 surface");
    makeTransparent();
}


} //namespace

