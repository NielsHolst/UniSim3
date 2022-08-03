/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Isaac Kwesi Abuley , Aarhus University [ikabuley at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "potato_development_base.h"

using namespace base;

namespace potato {

PotatoDevelopmentBase::PotatoDevelopmentBase(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(timeStepDays).imports("calendar[timeStepDays]");
    Output(step).help("The daily increment in the physiologic age of the potato");
    Output(total).help ("The accumulated daily physiologic age of the potato");
}

}
