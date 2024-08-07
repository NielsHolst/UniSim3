/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
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
    Input(avgPeriod).equals(7).unit(">0").help("Number of days to use for calculation");
    Input(min).equals(2.).unit("oC").help("Minimum soil temperature");
    Input(max).equals(40.).unit("oC").help("Maximum soil temperature");
    Input(outdoorsTemperature).imports("outdoors[temperature]");
    Input(atMidnight).imports("calendar[atMidnight]");
    Output(value).unit("oC").help("Soil temperature");
}

void SoilTemperature::reset() {
    // Set cycle buffer size
    _buf.resize(avgPeriod);
    // Start day anew
    _dailySum = 0.;
    _steps = 0;
    _doRefill = true;
}

void SoilTemperature::update() {
    // Daily update
    if (atMidnight && _steps > 0) {
        // Average of the past day
        const double avgTemperature = _dailySum/_steps;
        // First time do fill buffer
        if (_doRefill) {
            fillBuffer(avgTemperature);
            _doRefill = false;
        }
        // Afterwards, push daily average
        else {
            _buf.push(avgTemperature);
        }
        // Update to average of past days
        value = minmax(min, _buf.average(), max);
        // Start day anew
        _dailySum = 0;
        _steps = 0;

    }
    // Update daily sum
    _dailySum += outdoorsTemperature;
    ++_steps;
}

void SoilTemperature::fillBuffer(double temperature) {
    for (int i=0; i<avgPeriod; ++i)
        _buf.push(temperature);
}

} //namespace

