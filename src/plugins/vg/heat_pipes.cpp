/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "heat_pipes.h"

using namespace base;
using namespace vector_op;

namespace vg {

PUBLISH(HeatPipes)

HeatPipes::HeatPipes(QString name, Box *parent)
    : Box(name, parent)
{
    help("sums power use and radiation from growth light");
    Input(lwEmissionTopPipes).imports("./*[lwEmissionTop]");
    Input(lwEmissionBottomPipes).imports("./*[lwEmissionBottom]");
    Input(convectiveFluxTopPipes).imports("./*[parEmissionBottom]");
    Input(convectiveFluxBottomPipes).imports("./*[parEmissionBottom]");
    Output(lwEmissionTop).help("Long-wave emission upwards").unit("W/m2");
    Output(lwEmissionBottom).help("Long-wave emission downwards").unit("W/m2");
    Output(convectiveFluxTop).help("Convective heat flux upwards").unit("W/m2");
    Output(convectiveFluxBottom).help("Convective heat flux downwards").unit("W/m2");
}

void HeatPipes::reset() {
    update();
}

void HeatPipes::update() {
    lwEmissionTop     = sum(lwEmissionTopPipes);
    lwEmissionBottom  = sum(lwEmissionBottomPipes);
    convectiveFluxTop    = sum(convectiveFluxTopPipes);
    convectiveFluxBottom = sum(convectiveFluxBottomPipes);
}

} //namespace

