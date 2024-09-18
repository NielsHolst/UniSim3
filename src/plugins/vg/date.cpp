/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/convert.h>
#include <base/exception.h>
#include <base/publish.h>
#include "date.h"

using namespace base;

namespace boxes {

PUBLISH(Date)

Date::Date(QString name, Box *parent)
    : Box(name, parent)
{
    help("constructs a date from a DMY string, or from day, month and year");
    Input(dateString).unit("date").help("A date string");
    Input(day).equals(1).help("Day of month").unit("[1;31]");
    Input(month).equals(1).help("Month").unit("[1;12]");
    Input(year).equals(2000).help("Year");
    Output(date).help("The date constructed");
    NamedOutput("value", date).help("Synonym for `date`");
}


void Date::reset() {
    update();
}

void Date::update() {
    if (dateString.isEmpty())
        fromDetails();
    else
        fromString();
}

void Date::fromString() {
    date = convert<QDate>(dateString);
}

void Date::fromDetails(){
    date = QDate(year, month, day);
    if (!date.isValid()) {
        QString s("%1/%2/%3");
        ThrowException("Invalid date (y/m/d)").value(s.arg(year).arg(month).arg(day)) .context(this);
    }
}

}
