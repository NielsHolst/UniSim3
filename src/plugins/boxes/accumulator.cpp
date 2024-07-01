/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <limits>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "accumulator.h"

using namespace base;

namespace boxes {

PUBLISH(Accumulator)

Accumulator::Accumulator(QString name, Box *parent)
    : Box(name, parent)
{
    help("accumulates values");
    Input(initial).help("Initial `value` at reset");
    Input(change).help("Change added to `value`");
    Input(minValue).equals(std::numeric_limits<double>::lowest()).help("Minimum possible `value`");
    Input(maxValue).equals(std::numeric_limits<double>::max()).help("Maximum possible `value`");
    Output(value).help("Accumulated value");
    NamedOutput("signal", value).help("Synonym for `value`");
    Output(isAtMin).help("Is `value` equal to `minValue`?");
    Output(isAtMax).help("Is `value` equal to `maxValue`?");
}

void Accumulator::reset() {
    value = initial;
    change = 0.;
    update();
}

void Accumulator::update() {
    value += change;
    value = phys_math::minmax(minValue, value, maxValue);
    isAtMin = TestNum::eq(value, minValue);
    isAtMax = TestNum::eq(value, maxValue);
}

} //namespace

