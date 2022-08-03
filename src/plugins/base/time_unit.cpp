/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QMap>
#include "exception.h"
#include "node.h"
#include "time_unit.h"

using namespace base;

namespace Time {

QString toString(Unit unit) {
    static QMap<Unit, QString> lookup =
    {
        {Unit::Seconds, "s"},
        {Unit::Minutes, "m"},
        {Unit::Hours,   "h"},
        {Unit::Days,    "d"},
        {Unit::Years,   "y"}
    };
    return lookup.value(unit);
}

Unit toUnit(QString s, Node *concerning) {
    static QMap<QString, Unit> lookup =
    {
        {"s", Unit::Seconds },
        {"m", Unit::Minutes },
        {"h", Unit::Hours   },
        {"d", Unit::Days    },
        {"y", Unit::Years   }
    };
    if (!lookup.contains(s))
        ThrowException("Unknown time unit").value(s).context(concerning);
    return lookup.value(s);
}


int toSeconds(Unit unit) {
    static QMap<Unit, int> lookup =
    {
        {Unit::Seconds, 1},
        {Unit::Minutes, 60},
        {Unit::Hours,   60*60},
        {Unit::Days,    24*60*60},
        {Unit::Years,   365*24*60*60}
    };
    return lookup.value(unit);
}

int toSeconds(QString unit) {
    return toSeconds(Time::toUnit(unit));
}

double toDays(Unit unit) {
    return toSeconds(unit)/double(toSeconds(Unit::Days));
}

double toDays(QString unit) {
    return toDays(Time::toUnit(unit));
}

QDateTime operator+(const QDateTime &dateTime, Period period) {
    switch (period.unit) {
    case Unit::Seconds:
        return dateTime.addSecs(period.time);
    case Unit::Minutes:
        return dateTime.addSecs(60*period.time);
    case Unit::Hours:
        return dateTime.addSecs(60*60*period.time);
    case Unit::Days:
        return dateTime.addDays(period.time);
    case Unit::Years:
        QDate date = dateTime.date();
        QTime savedTime = dateTime.time();
        date = QDate(date.year() + period.time, date.month(), date.day());
        return QDateTime(date, savedTime, dateTime.timeSpec());
    }
    return QDateTime();
}

QDateTime operator-(const QDateTime &dateTime, Period period) {
    return dateTime + Period(-period.time, period.unit);
}

}

