/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include <base/vector_op.h>
#include "pad_and_fans.h"

using namespace base;
using namespace vector_op;

namespace vg {

PUBLISH(PadAndFans)

PadAndFans::PadAndFans(QString name, Box *parent)
    : Box(name, parent)
{
    help("sums heat pump outputs");
    Input(powerUses   ).imports("./*[powerUse]");
    Input(coolings    ).imports("./*[cooling]");
    Input(vapourFluxes).imports("./*[vapourFlux]");
    Output(powerUse).help("Power use including parasitic").unit("W/m2");
    Output(cooling).help("Cooling achieved by evaporation").unit("W/m2");
    Output(vapourFlux).help("Rate of evaporation").unit("kg/m2/s");
}

void PadAndFans::reset() {
    update();
}

void PadAndFans::update() {
    powerUse   = sum(powerUses);
    cooling    = sum(coolings);
    vapourFlux = sum(vapourFluxes);
}

} //namespace

