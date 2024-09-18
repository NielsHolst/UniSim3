/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
/* From Orgill, J. F. and K. G. T. Hollands (1977). "Correlation equation for hourly diffuse radiation on a horizontal surface."
 * Solar Energy 19(4): 357-359.
*/
#include <base/publish.h>
#include "sun_diffuse_radiation.h"

using namespace base;

namespace boxes {

PUBLISH(SunDiffuseRadiation)

SunDiffuseRadiation::SunDiffuseRadiation(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes proportion diffuse global radiation");
    Input(globalRadiation).unit("W/2").help("Global radiation");
    Input(angotRadiation).imports("sun[angot]");
    Output(value).unit("[0;1]").help("Proportion diffuse light");
}

void SunDiffuseRadiation::reset() {
    update();
}

void SunDiffuseRadiation::update() {
    const double kt = globalRadiation/angotRadiation;
    if (kt < 0.35)
        value = 1. - 0.249*kt;
    else if (kt < 0.75)
        value = 1.557 - 1.84*kt;
    else
        value = 0.177;
}

} //namespace

