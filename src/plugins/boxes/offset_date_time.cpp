/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "offset_date_time.h"
using namespace base;

namespace boxes {

PUBLISH(OffsetDateTime)

OffsetDateTime::OffsetDateTime(QString name, Box *parent)
    : Box(name, parent) {
    help("computes a date-time plus/minus an offset");
    Input(dateTime).help("Date-time that will be offset");
    Input(days).help("Offset (negative or positive)").unit("d");
    Input(hours).help("Offset (negative or positive)").unit("h");
    Input(minutes).help("Offset (negative or positive)").unit("m");
    Output(value).help("Offset date-time");
}

void OffsetDateTime::reset() {
    update();
}

void OffsetDateTime::update() {
    value = dateTime.addDays(days).addSecs(60*hours + minutes);
}

} //namespace

