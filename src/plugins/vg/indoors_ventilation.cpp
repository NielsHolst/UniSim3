/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "indoors_ventilation.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(IndoorsVentilation)

IndoorsVentilation::IndoorsVentilation(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes total air flux from outdoors to indoors");
    Input(fluxLeakageAlone).imports("./leakage[flux]", CA);
    Input(fluxVentilationAlone).imports("actuators/ventilation[flux]", CA);
    Input(screensAirTransmissivity).imports("shelter[screensAirTransmissivity]", CA);
    Output(flux).help("Total air flux").unit("/h");
    Output(fluxLeakage).help("Leakage air flux").unit("/h");
    Output(fluxVentilation).help("Ventilation air flux").unit("/h");
}

void IndoorsVentilation::reset() {
    update();
}

void IndoorsVentilation::update() {
    flux = screensAirTransmissivity*sqrt(p2(fluxLeakageAlone) + p2(fluxVentilationAlone));
    if (flux > 0.) {
        double linearTotal = fluxLeakageAlone + fluxVentilationAlone;
        fluxLeakage     = flux*fluxLeakageAlone    /linearTotal;
        fluxVentilation = flux*fluxVentilationAlone/linearTotal;
    }
    else
        fluxLeakage =
        fluxVentilation = 0.;
}

} //namespace

