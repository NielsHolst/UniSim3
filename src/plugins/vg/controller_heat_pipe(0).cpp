/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "controller_heat_pipe.h"

using namespace base;

namespace vg {
	
PUBLISH(ControllerHeatPipe)

ControllerHeatPipe::ControllerHeatPipe(QString name, Box *parent)
    : Box(name, parent), isOn(false)
{
    help("controls heat pipe temperature");
    Input(terms).unit("oC").help("Vector of setpoint temperatures to be summed");
    Input(minTemperature).imports("setpoints/heating/minTemperature[value]");
    Input(maxTemperature).imports("setpoints/heating/maxTemperature[value]");
    Output(temperature).help("Desired heat pipe temperature").unit("oC");
}

void ControllerHeatPipe::reset() {
    update();
}

void ControllerHeatPipe::update() {
    temperature = phys_math::minmax(minTemperature, vector_op::sum(terms), maxTemperature);
}

} //namespace

