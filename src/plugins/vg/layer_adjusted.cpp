/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "layer_adjusted.h"

using namespace base;

namespace vg {

PUBLISH(LayerAdjusted)

LayerAdjusted::LayerAdjusted(QString name, Box *parent)
    : Box(name, parent)
{
    help("holds adjusted radiative and heat parameters of a layer");
    const char text[] = "Parameter adjusted for area, etc.";
    Output(swReflectivityTopAdj).unit("[0;1]").help(text);
    Output(swReflectivityBottomAdj).unit("[0;1]").help(text);
    Output(swTransmissivityTopAdj).unit("[0;1]").help(text);
    Output(swTransmissivityBottomAdj).unit("[0;1]").help(text);
    Output(lwReflectivityTopAdj).unit("[0;1]").help(text);
    Output(lwReflectivityBottomAdj).unit("[0;1]").help(text);
    Output(lwTransmissivityTopAdj).unit("[0;1]").help(text);
    Output(lwTransmissivityBottomAdj).unit("[0;1]").help(text);
    Output(swAbsorptivityTopAdj).unit("[0;1]").help(text);
    Output(swAbsorptivityBottomAdj).unit("[0;1]").help(text);
    Output(lwAbsorptivityTopAdj).unit("[0;1]").help(text);
    Output(lwAbsorptivityBottomAdj).unit("[0;1]").help(text);
    Output(UtopAdj).unit("W/K/m2 ground").help(text);
    Output(UbottomAdj).unit("W/K/m2 ground").help(text);
    Output(heatCapacityAdj).unit("J/K/m2 ground").help(text);

}

} //namespace

