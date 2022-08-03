/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DATE_TIME_SIGNAL_H
#define DATE_TIME_SIGNAL_H

#include <base/bare_date.h>
#include <base/base_signal.h>

namespace boxes {

class DateTimeSignal : public base::BaseSignal
{
public:
    DateTimeSignal(QString name, Box *parent);
    bool computeFlag();
    double computeSignal(bool flag);
private:
    // Inputs
    base::BareDate beginDate, endDate;
    QTime beginTime, endTime;
    QDateTime currentDateTime;
    double signalInside, signalOutside;
    bool circadian;
};

} //namespace
#endif
