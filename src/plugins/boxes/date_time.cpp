/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/convert.h>
#include <base/exception.h>
#include <base/publish.h>
#include "date_time.h"

using namespace base;

namespace boxes {

PUBLISH(DateTime)

DateTime::DateTime(QString name, Box *parent)
    : Box(name, parent)
{
    help("constructs a datetime from a DMYHMS string, or from day, month, year, hour, minute and second");
    Input(dateTimeString).unit("datetime").help("A datetime string");
    Input(day).equals(1).help("Day of month").unit("[1;31]");
    Input(month).equals(1).help("Month").unit("[1;12]");
    Input(year).equals(2000).help("Year");
    Input(hour).help("Hour of the day").unit("[0:24]");
    Input(minute).help("Minute of the hour").unit("[0:59]");
    Input(second).help("Second of the minute").unit("[0:59]");
    Output(dateTime).help("The datetime constructed");
    NamedOutput("value", dateTime).help("Synonym for `dateTime`");
}


void DateTime::reset() {
    update();
}

void DateTime::update() {
    if (dateTimeString.isEmpty())
        fromDetails();
    else
        fromString();
}

void DateTime::fromString() {
    dateTime = convert<QDateTime>(dateTimeString);
}

void DateTime::fromDetails(){
    QDate date = QDate(year, month, day);
    if (!date.isValid()) {
        QString s("%1/%2/%3");
        ThrowException("Invalid date (y/m/d)").value(s.arg(year).arg(month).arg(day)) .context(this);
    }
    QTime time = QTime(hour, minute, second);
    if (!time.isValid()) {
        QString s("%1:%2:%3");
        ThrowException("Invalid time (H:M:S)").value(s.arg(hour).arg(minute).arg(second)) .context(this);
    }
    dateTime = QDateTime(date, time);
}

}
