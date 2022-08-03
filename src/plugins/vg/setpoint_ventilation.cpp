/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include "setpoint_ventilation.h"

using namespace base;

namespace vg {

PUBLISH(SetpointVentilation)

SetpointVentilation::SetpointVentilation(QString name, Box *parent)
    : Box(name, parent)
{
    help("models the temperature setpoint for ventilation");
    Input(setpointHeating).imports("../heatingTemperature[value]", CA);
    Input(ventilationTemperatureMargin).imports("..[ventilationTemperatureMargin]", CA);
    Output(value).help("Temperature setpoint for ventilation").unit("oC");
}

void SetpointVentilation::reset() {
    update();
}

void SetpointVentilation::update() {
    value = setpointHeating + ventilationTemperatureMargin;
}

} //namespace

