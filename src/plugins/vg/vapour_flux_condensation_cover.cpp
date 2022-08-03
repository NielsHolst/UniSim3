/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "vapour_flux_condensation_cover.h"

using std::min;
using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(VapourFluxCondensationCover)

VapourFluxCondensationCover::VapourFluxCondensationCover(QString name, Box *parent)
    : VapourFluxBase(name, parent)
{
    help("models vapour flux by condensation on inside greenhouse surface");
    Input(coverPerGroundArea).imports("geometry[coverPerGroundArea]");
    Input(coverTemperature).imports("energyBudget/cover[temperature]",CA);
    Input(indoorsTemperature).imports("indoors/temperature[value]",CA);
    Input(indoorsAh).imports("indoors/humidity[ah]",CA);
    Output(surfaceSah).help("Saturated absolute humidity at cover").unit("kg/m3");
}

void VapourFluxCondensationCover::update() {
    double dTemp = indoorsTemperature - coverTemperature;
    conductance = dTemp > 0
                  ? coverPerGroundArea*1.64e-3*pow(dTemp, 1/3.) // GCC, p.147
                  : 0.;
    surfaceSah = sah(coverTemperature);
    vapourFlux = min(conductance*(surfaceSah - indoorsAh), 0.);
    gain = conductance*surfaceSah;
}

} //namespace

