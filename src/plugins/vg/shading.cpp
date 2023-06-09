/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "shading.h"

using namespace base;

namespace vg {

PUBLISH(Shading)

Shading::Shading(QString name, Box *parent)
    : Box(name, parent)
{
    help("models reflection caused by whitening");
    Input(swReflectivities).imports("./*[swReflectivity]");
    Input(lwReflectivities).imports("./*[lwReflectivity]");
    Input(states).imports("./*[state]");
    Output(swReflectivity).help("Short-wave reflectivity added to cover").unit("[0;1]");
    Output(lwReflectivity).help("Long-wave reflectivity added to cover").unit("[0;1]");
}

void Shading::reset() {
    swReflectivity =
    lwReflectivity = 0.;
}

void Shading::update() {
    double swNot = 1.,
           lwNot = 1.;
    int n = states.size();
    for (int i = 0; i < n; ++i) {
        swNot *= 1. - states.at(i)*swReflectivities.at(i);
        lwNot *= 1. - states.at(i)*lwReflectivities.at(i);
    }
    swReflectivity = 1. - swNot;
    lwReflectivity = 1. - lwNot;
}

} //namespace

