/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_BARE_DATE_H
#define BASE_BARE_DATE_H
#include <QDate>

namespace base {

class BareDate {
public:
    static const int baseYear = 2000;

    BareDate(int m, int d) : _date(baseYear, m, d) {}
    explicit BareDate(QDate date) : _date(baseYear, date.month(), date.day()) {}
    BareDate() : _date() {}
    BareDate addBareDays(int days) const {
        // Wrap around at end of year
        QDate newDate = _date.addDays(days);
        return BareDate(newDate.month(), newDate.day());
    }
    QDate date() const { return _date; }
    int month() const  { return _date.month(); }
    int day() const  { return _date.day(); }
private:
    QDate _date;
};

// Addition

BareDate operator+(const BareDate &bare, double x);
BareDate operator+(double x, const BareDate &bare);

// Subtraction

BareDate operator-(const BareDate &bare, double x);
int operator-(const BareDate &a, const BareDate &b);

// Comparisons between two BareDate objects

inline bool  operator==(const BareDate &a, const BareDate &b) {
    return (a.date() == b.date());
}

inline bool  operator!=(const BareDate &a, const BareDate &b) {
    return (a.date() != b.date());
}

inline bool operator<(const BareDate &a, const BareDate &b) {
    return a.date() < b.date();
}

inline bool operator<=(const BareDate &a, const BareDate &b) {
    return a.date() <= b.date();
}

inline bool operator>(const BareDate &a, const BareDate &b) {
    return a.date() > b.date();
}

inline bool operator>=(const BareDate &a, const BareDate &b) {
    return a.date() >= b.date();
}

// Comparisons between a BareDate and a QDate object

inline bool operator==(const BareDate &a, const QDate &b) {
    return (a.month() == b.month() && a.day() == b.day());
}
inline bool operator==(const QDate &a, const BareDate &b) {
    return b==a;
}

inline bool operator!=(const BareDate &a, const QDate &b) {
    return (a.month() != b.month() || a.day() != b.day());
}
inline bool operator!=(const QDate &a, const BareDate &b) {
    return b!=a;
}

inline bool operator<(const BareDate &a, const QDate &b) {
    return a.date() < QDate(BareDate::baseYear, b.month(), b.day());
}

inline bool operator<=(const BareDate &a, const QDate &b) {
    return a.date() <= QDate(BareDate::baseYear, b.month(), b.day());
}

inline bool operator>(const BareDate &a, const QDate &b) {
    return a.date() > QDate(BareDate::baseYear, b.month(), b.day());
}
inline bool operator>(const QDate &a, const BareDate &b) {
    return b<a;
}

inline bool operator>=(const BareDate &a, const QDate &b) {
    return a.date() >= QDate(BareDate::baseYear, b.month(), b.day());
}
inline bool operator>=(const QDate &a, const BareDate &b) {
    return b<=a;
}

inline bool operator<(const QDate &a, const BareDate &b) {
    return b>a;
}

inline bool operator<=(const QDate &a, const BareDate &b) {
    return b>=a;
}

}

#endif

