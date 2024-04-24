/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "humidification_controller.h"

using namespace base;

namespace vg {
	
PUBLISH(HumidificationController)

HumidificationController::HumidificationController(QString name, Box *parent)
    : Box(name, parent)
{
    help("controls humidifiers according to setpoint");
    Input(spState).imports("setpoints/humidification/state[value]");
    Output(state).unit("[0;1]").help("Desired state of humifiers");
}

void HumidificationController::reset() {
    spState = 0.;
    update();
}

void HumidificationController::update() {
    state = spState;
}

} //namespace

