/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "actuator_chalk.h"

using namespace base;

namespace vg {

PUBLISH(ActuatorChalk)

ActuatorChalk::ActuatorChalk(QString name, Box *parent)
    : Box(name, parent)
{
    help("models reflection caused by whitening");
    Input(state).imports("gh/setpoints/chalk[value]");
    Input(swReflectivity).help("Short-wave reflectivity potentially added to cover").unit("[0;1]");
    Input(lwReflectivity).help("Long-wave reflectivity potentially added to cover").unit("[0;1]");
    Output(swReflectivityValue).help("Short-wave reflectivity actually added to cover").unit("[0;1]");
    Output(lwReflectivityValue).help("Long-wave reflectivity actually added to cover").unit("[0;1]");
}

void ActuatorChalk::reset() {
    update();
}

void ActuatorChalk::update() {
    swReflectivityValue = state*swReflectivity;
    lwReflectivityValue = state*lwReflectivity;
}

} //namespace

