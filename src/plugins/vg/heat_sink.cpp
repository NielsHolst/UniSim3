/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "heat_sink.h"
#include "indoors_temperature.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(HeatSink)

HeatSink::HeatSink(QString name, Box *parent)
    : Box(name, parent)
{
    help("provides an artificial sink for excess heat");
    Input(setpointVentilation).imports("setpoints/ventilationTemperature[value]", CA);
    Input(indoorsTemperature).imports("indoors/temperature[value]", CA);
    Input(averageHeight).imports("geometry[averageHeight]", CA);
    Input(dt).imports("calendar[timeStepSecs]", CA);
    Output(value).help("Heat flux to sink").unit("W/m2");
}

void HeatSink::initialize() {
    indoorsTemperatureBox  = findOne<IndoorsTemperature*>("indoors/temperature");
}

void HeatSink::reset() {
    update();
}

void HeatSink::update() {
    double deltaT = indoorsTemperature - setpointVentilation;
    if (deltaT > 0.) {
        value =  CpAirVol*deltaT*averageHeight/dt;  // (J/m3/K) * K * m / s = W/m2
        indoorsTemperatureBox->setTemperature(setpointVentilation);
    }
    else
        value = 0.;
}

} //namespace

