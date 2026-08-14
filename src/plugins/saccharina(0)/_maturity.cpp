/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "maturity.h"

using namespace base;

namespace saccharina {

PUBLISH(Maturity)

Maturity::Maturity(QString name, QObject *parent)
    : Box(name, parent)
{
    help("calculates plant maturity as an index between 0 and 1");
    Input(A).imports("area[value]");
    Input(a50).equals(3.).unit("dm2").help("Area at which erosion reaches 50%");
    Input(aSlope).equals(8.).help("Slope at A50");
    Input(forced).equals(999).help("If inside [0;1] then value is forced to that, otherwise ignored");
    Output(value).unit("[0;1]").help("Maturity index");
}

void Maturity::reset() {
    isForced = TestNum::geZero(forced) && TestNum::le(forced, 1.);
    update();
}

void Maturity::update() {
    if (isForced)
        value = forced;
    else
        value = (A == 0.) ? 1 : 1/(1. + exp(-aSlope*(log(A) - log(a50))));
}

}

