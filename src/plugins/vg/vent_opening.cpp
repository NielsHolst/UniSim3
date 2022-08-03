/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "vent_opening.h"

using namespace base;
using std::min;

namespace vg {

PUBLISH(VentOpening)

VentOpening::VentOpening(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes supply of air flux to cool greenhouse");
    Input(airFluxSupply).imports("../airFlux[value]",CA);
    Input(airFluxSupplyMax).imports("cooling/airSupplyMax[value]",CA);
    Input(ventOpeningExponent).equals(2.).help("Exponent converting supply/supplyMax ratio to vent opening [0;1]");
    Output(value).help("Relative vent opening [0;1]");
}

void VentOpening::update() {
    TestNum::snapTo(airFluxSupplyMax, 0.);
    value = (airFluxSupplyMax > 0.) ? airFluxSupply/airFluxSupplyMax : 0.;
    TestNum::snapTo(value, 0.);
    TestNum::snapTo(value, 1.);
    if (value<.0 || value>1.) {
        QString s = "airFluxSupply (%1)/airFluxSupplyMax(%2)";
        ThrowException("Unexpected vent opening; must be inside [0;1]").value(value).
                value2(s.arg(airFluxSupply).arg(airFluxSupplyMax)).context(this);
    }
}

} //namespace

