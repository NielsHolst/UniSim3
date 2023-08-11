/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "date_time_signal.h"

using namespace base;

namespace boxes {

PUBLISH(DateTimeSignal)

DateTimeSignal::DateTimeSignal(QString name, Box *parent)
    : BaseSignal(name, parent){
    help("sets a signal according to date (ignoring year) and time of day");
    Input(currentDateTime).imports("calendar[dateTime]");
    Input(beginDate).equals(BareDate(1,1)).help("The beginning of the date interval");
    Input(endDate).equals(BareDate(12,31)).help("The end of the date interval");
    Input(beginTime).equals(QTime(0,0)).help("The beginning of the time interval");
    Input(endTime).equals(QTime(0,0)).help("The end of the time interval");
    Input(signalInside).equals(1.).help("Resulting `signal` when day and time are inside the day and time intervals");
    Input(signalOutside).equals(0.).help("Resulting `signal` when day and time are outside the day and time intervals");
    Input(circadian).equals(true).help("Tells whether `signal` follows a daily rythm");
}

bool DateTimeSignal::computeFlag() {
    const QDate &today = currentDateTime.date();
    const QTime &time  = currentDateTime.time();
    bool
        flag,
        nowOnDay = (beginDate < endDate) ?
                (today >= beginDate) && (today <= endDate) :
                (today >= beginDate) || (today <= endDate),
        ignoreTime = (beginTime == QTime(0,0) && endTime == QTime(0,0)),
        nowOnTime = ignoreTime ||
            (   (beginTime < endTime) ?
                    (time >= beginTime) && (time <= endTime) :
                    (time >= beginTime) || (time <= endTime) );
    if (circadian) {
        flag =  nowOnDay && nowOnTime;
    }
    else {
        flag = nowOnDay && (ignoreTime ||
                             (
                               (today != beginDate || time >= beginTime) &&
                               (today != endDate   || time <= endTime)
                             )
                           );
    }
    return flag;
}

double DateTimeSignal::computeSignal(bool flag) {
    return flag ? signalInside : signalOutside;
}

} //namespace

