/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "soil_temperature.h"

using namespace base;
using phys_math::minmax;

namespace vg {

PUBLISH(SoilTemperature)

SoilTemperature::SoilTemperature(QString name, Box *parent)
    : Box(name, parent)
{
    help("models soil temperature under the greenhouse");
    Input(setpointHeating).imports("setpoints/heatingTemperature[value]");
    Input(indoorsTemperature).imports("indoors/temperature[value]");
    Input(outdoorsTemperature).imports("outdoors[temperature]");
    Input(atMidnight).imports("calendar[atMidnight]");
    Input(avgOutdoorsTemperature).equals(5.).unit("oC").help("Average temperature of month before simulation start");
    Input(minValue).equals(2.).unit("oC").help("Minimum soil temperature");
    Input(maxValue).equals(40.).unit("oC").help("Maximum soil temperature");
    Output(value).unit("oC").help("Soil temperature");
}

void SoilTemperature::reset() {
    // Fill buffer with 30 default values
    value = (setpointHeating + avgOutdoorsTemperature)/2.;
    value = avgOutdoorsTemperature;
    _buf.resize(7);
    for (int i=0; i<7; ++i)
        _buf.push(value);
    // Start day anew
    _dailySum = 0.;
    _steps = 0;
}

void SoilTemperature::update() {
    // Daily update
    if (atMidnight && _steps > 0) {
        // Push daily average
        _buf.push(_dailySum/_steps);
        // Report 30-days average
        value = minmax(minValue, _buf.average(), maxValue);
        // Start day anew
        _dailySum = 0;
        _steps = 0;

    }
    // Always update daily sum
    _dailySum += outdoorsTemperature; // + indoorsTemperature;
    ++_steps;
}

} //namespace

