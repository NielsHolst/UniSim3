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
    Input(initial).equals(5.).unit("oC").help("Initial soil temperature");
    Input(min).equals(2.).unit("oC").help("Minimum soil temperature");
    Input(max).equals(40.).unit("oC").help("Maximum soil temperature");
    Input(outdoorsTemperature).imports("outdoors[temperature]");
    Input(atMidnight).imports("calendar[atMidnight]");
    Output(value).unit("oC").help("Soil temperature");
}

void SoilTemperature::reset() {
    // Fill buffer with initial values
    value = initial;
    _buf.resize(_bufSize);
    for (int i=0; i<_bufSize; ++i)
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
        // Report buffer average
        value = minmax(min, _buf.average(), max);
        // Start day anew
        _dailySum = 0;
        _steps = 0;

    }
    // Update daily sum
    _dailySum += outdoorsTemperature;
    ++_steps;
}

} //namespace

