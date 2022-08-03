/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "ratio.h"

using namespace base;

namespace boxes {

PUBLISH(Ratio)

Ratio::Ratio(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes a ratio; 0/0 yields 0");
    Input(denominator).help("Number to be divided");
    Input(divisor).help("The divisor");
    Input(zeroTolerance).help("Used for infinity check");
    Input(allowInfinityResult).equals(true).help("Will yield an error of divisor<=zeroTolerance");
    Output(value).help("The ratio");
}

void Ratio::reset() {
    update();
}

void Ratio::update() {
    if (denominator == 0.)
        value = 0.;
    else {
        if (!allowInfinityResult && divisor<=zeroTolerance)
            ThrowException("Divisor too close to zero tolerance")
                    .value(divisor).value1("is <=").value2(zeroTolerance);
        value =  denominator/divisor;
    }
}

}
