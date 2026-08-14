/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "soil_nitrogen.h"

using namespace base;
using namespace std;

namespace coffee {

PUBLISH(SoilNitrogen)

SoilNitrogen::SoilNitrogen(QString name, QObject *parent)
    : Box(name, parent) {
    help("maintains mineral soil nitrogen");
    Input(initial).equals(0.001).unit("kg N/m2").help("Initial mineral soil nitrogen");
    Input(fertilisation).equals(0.).unit("kg N/m2");
    Input(mineralisation).equals(0.).unit("kg N/m2");
    Input(leaching).equals(0.).unit("kg N/m2");
    Input(emission).equals(0.).unit("kg N/m2");
    Input(uptake).imports("nitrogenSupply[value]");
    Output(value).unit("kg N/m2").help("Mineral soil nitrogen");
}

void SoilNitrogen::reset() {
    value = initial;
}

void SoilNitrogen::update() {
    value += fertilisation + mineralisation - uptake - leaching - emission;
}

} //namespace

