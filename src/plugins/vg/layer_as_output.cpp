/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include "layer_as_output.h"

using namespace base;

namespace vg {

LayerAsOutput::LayerAsOutput(QString name, Box *parent)
    : Layer(name, parent)
{
    help("holds radiative and heat parameters of a layer used for parameter output");
    Output(swAbsorptivityTop).unit("[0;1]").help("Short-wave absorptivity at the top");
    Output(swReflectivityTop).unit("[0;1]").help("Short-wave reflectivity at the top");
    Output(swTransmissivityTop).equals(1.).unit("[0;1]").help("Short-wave transmissivity at the top");

    Output(swAbsorptivityBottom).unit("[0;1]").help("Short-wave absorptivity at the bottom");
    Output(swReflectivityBottom).unit("[0;1]").help("Short-wave reflectivity at the bottom");
    Output(swTransmissivityBottom).equals(1.).unit("[0;1]").help("Short-wave transmissivity at the bottom");

    Output(lwAbsorptivityTop).unit("[0;1]").help("Long-wave absorptivity at the top");
    Output(lwReflectivityTop).unit("[0;1]").help("Long-wave reflectivity at the top");
    Output(lwTransmissivityTop).equals(1.).unit("[0;1]").help("Long-wave transmissivity at the top");

    Output(lwAbsorptivityBottom).unit("[0;1]").help("Long-wave absorptivity at the bottom");
    Output(lwReflectivityBottom).unit("[0;1]").help("Long-wave reflectivity at the bottom");
    Output(lwTransmissivityBottom).equals(1.).unit("[0;1]").help("Long-wave transmissivity at the bottom");

    // When used as output these ports are per ground area
    Output(Utop).unit("W/K/m2 ground").help("Heat transfer coefficient at the top");
    Output(Ubottom).unit("W/K/m2 ground").help("Heat transfer coefficient at the bottom");
    Output(heatCapacity).unit("J/K/m2 ground").help("Area-specific heat capacity");
    makeTransparent();
}


} //namespace

