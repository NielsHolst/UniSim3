/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_TIME_UNIT_H
#define BASE_TIME_UNIT_H
#include <QDateTime>
#include <QString>

namespace base {
class Node;
}

namespace Time {

enum class Unit{Seconds, Minutes, Hours, Days, Years};

QString toString(Unit unit);
Unit toUnit(QString s, base::Node *concerning = nullptr);
int toSeconds(Unit unit);
int toSeconds(QString unit);
double toDays(Unit unit);
double toDays(QString unit);

struct Period {
    Period(int t, Unit u) : time(t), unit(u) {}
    Period(int t, QString u) : time(t), unit(toUnit(u)) {}
    int time;
    Unit unit;
};

QDateTime operator+(const QDateTime &dateTime, Period period);
QDateTime operator-(const QDateTime &dateTime, Period period);

} //namespace

#endif
