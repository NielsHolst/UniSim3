/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "humidifiers.h"

using namespace base;
using namespace vector_op;

namespace vg {

PUBLISH(Humidifiers)

Humidifiers::Humidifiers(QString name, Box *parent)
    : Box(name, parent)
{
    help("sums heat pump outputs");
    Input(powerUses   ).imports("./*[powerUse]");
    Input(vapourFluxes).imports("./*[vapourFlux]");
    Output(powerUse).help("Power use (total)").unit("W/m2");
    Output(vapourFlux).help("Vapour flux (total)").unit("kg/m2/s");
}

void Humidifiers::reset() {
    update();
}

void Humidifiers::update() {
    powerUse   = sum(powerUses);
    vapourFlux = sum(vapourFluxes);
}

} //namespace

