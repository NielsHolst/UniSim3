/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_TIME_UNIT_H
#define BASE_TIME_UNIT_H

#include <QDateTime>
#include <QString>

#include "base_dllexport.h"

namespace base {
class Node;
}

namespace Time {

enum class UNI_API Unit{Seconds, Minutes, Hours, Days, Years};

UNI_API QString toString(Unit unit);
UNI_API Unit toUnit(QString s, base::Node *concerning = nullptr);
UNI_API int toSeconds(Unit unit);
UNI_API int toSeconds(QString unit);
UNI_API double toDays(Unit unit);
UNI_API double toDays(QString unit);

struct UNI_API Period {
    Period(int t, Unit u) : time(t), unit(u) {}
    Period(int t, QString u) : time(t), unit(toUnit(u)) {}
    int time;
    Unit unit;
};

UNI_API QDateTime operator+(const QDateTime &dateTime, Period period);
UNI_API QDateTime operator-(const QDateTime &dateTime, Period period);

} //namespace

#endif
