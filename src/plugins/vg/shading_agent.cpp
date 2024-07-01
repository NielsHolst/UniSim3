/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "shading_agent.h"

using namespace base;

namespace vg {

PUBLISH(ShadingAgent)

ShadingAgent::ShadingAgent(QString name, Box *parent)
    : Box(name, parent)
{
    help("models reflection caused by whitening");
    Input(swReflectivity).help("Short-wave reflectivity potentially added to cover").unit("[0;1]");
    Input(lwReflectivity).help("Long-wave reflectivity potentially added to cover").unit("[0;1]");
    Input(state).help("State of shading agent").unit("[0;1]");
}

} //namespace

