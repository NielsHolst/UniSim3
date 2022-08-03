/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include "setpoint_heating.h"

using namespace base;

namespace vg {

PUBLISH(SetpointHeating)

SetpointHeating::SetpointHeating(QString name, Box *parent)
    : Box(name, parent)
{
    help("models the temperature setpoint for heating");
    Input(heatingTemperatureNormalRh).imports("..[heatingTemperatureNormalRh]", CA);
    Input(heatingTemperatureHighRhIncrement).imports("../heatingTemperatureHighRhIncrement[value]", CA);
    Input(heatingTemperatureLowRhIncrement). imports("../heatingTemperatureLowRhIncrement[value]", CA);
    Output(value).help("Temperature setpoint for heating").unit("oC");
}

void SetpointHeating::reset() {
    update();
}

void SetpointHeating::update() {
    value = heatingTemperatureNormalRh +
            heatingTemperatureHighRhIncrement +
            heatingTemperatureLowRhIncrement;
}

} //namespace

