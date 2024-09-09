/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/convert.h>
#include <base/publish.h>
#include "time_to_hours.h"

using namespace base;

namespace boxes {

PUBLISH(TimeToHours)

TimeToHours::TimeToHours(QString name, Box *parent)
    : Box(name, parent)
{
    help("converts time to hours since midnight");
    Input(time).equals(QTime(0,0,0)).unit("time").help("Time of day");
    Output(value).help("Hours since midnight");
}

void TimeToHours::reset() {
    update();
}

void TimeToHours::update() {
    value = convert<double>(time);
}

}
