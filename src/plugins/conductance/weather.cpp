/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "weather.h"

using namespace base;

namespace conductance {

PUBLISH(Weather)

Weather::Weather(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(irradiation).equals(10).help("Global irradiation (MJ/m^2/d)");
}

} //namespace

