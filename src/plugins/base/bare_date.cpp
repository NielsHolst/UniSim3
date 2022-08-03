/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <boost/numeric/conversion/cast.hpp>
#include "bare_date.h"
#include "convert.h"

namespace base {

BareDate operator-(const BareDate &bare, double x) {
    QDate date = bare.date().addDays(-convert<int>(x));
    return BareDate(date.month(), date.day());
}

int operator-(const BareDate &a, const BareDate &b) {
    return boost::numeric_cast<int>(b.date().daysTo(a.date()));
}

BareDate operator+(const BareDate &bare, double x) {
    QDate date = bare.date().addDays(convert<int>(x));
    return BareDate(date.month(), date.day());
}

BareDate operator+(double x, const BareDate &bare) {
    return bare + x;
}

}
