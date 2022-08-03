/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "vapour_flux_ventilation.h"

using namespace base;

namespace vg {

PUBLISH(VapourFluxVentilation)

VapourFluxVentilation::VapourFluxVentilation(QString name, Box *parent)
    : VapourFluxBase(name, parent)
{
    help("models vapour flux by air exchange");
    Input(timeStep).imports("calendar[timeStepSecs]");
    Input(airInflux).imports("indoors/ventilation[flux]", CA);
    Input(averageHeight).imports("geometry[averageHeight]", CA);
    Input(indoorsAh).imports("indoors/humidity[ah]",CA);
    Input(outdoorsAh).imports("outdoors[ah]",CA);
}

void VapourFluxVentilation::update() {
    conductance = averageHeight*airInflux/3600;          // m/s = m /h * h/s
    vapourFlux = conductance*(outdoorsAh-indoorsAh);     // kg/m2/s = m/s * kg/m3
    gain = conductance*outdoorsAh;                       // kg/m2/s = m/s * kg/m3
}

} //namespace

