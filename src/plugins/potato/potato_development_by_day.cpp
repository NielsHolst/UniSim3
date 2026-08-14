/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Isaac Kwesi Abuley , Aarhus University [ikabuley at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "potato_development_by_day.h"

using namespace base;

namespace potato {

PUBLISH(PotatoDevelopmentByDay)

PotatoDevelopmentByDay::PotatoDevelopmentByDay(QString name, QObject *parent)
    : PotatoDevelopmentBase(name, parent)
{
    help("models development based on daily min-max temperatures");
    Input(minStep).imports("./min[step]").help("The Physiologic Development at the minimum temperature in a day");
    Input(maxStep).imports("./max[step]").help("The Physiologic Development at the maximum temperature in a day");
}

void PotatoDevelopmentByDay::reset() {
   total = 0.;
   update();
}

void PotatoDevelopmentByDay::update()
{
    step = 1./24.*(5.*minStep+(8.*(2.*minStep/3.+maxStep/3.)) + 8.*(2.*maxStep/3.+minStep/3.) + 3.*maxStep);
    step *= timeStepDays;
    total += step;
}

}
