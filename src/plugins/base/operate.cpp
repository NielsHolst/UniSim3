/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <boost/numeric/conversion/cast.hpp>
#include <QDate>
#include <QDateTime>
#include <QString>
#include <QTime>
#include "bare_date.h"
#include "convert.h"
#include "exception.h"
#include "operate.h"

using namespace base;
using boost::numeric_cast;

namespace operate {

// Exponentiation by squaring
// stackoverflow.com/questions/101439/the-most-efficient-way-to-implement-an-integer-based-power-function-powint-int

int ipow(int base, int exp)
{
    // Special cases
    bool isNegative = (base<0 && exp%2==1);
    if (base == 0)
        if (exp == 0)
            ThrowException("0^0 is illegal");
        else
            return 0;
    else if (exp == 0)
        return 1;
    // Exponentiate
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }
    // Correct for sign
    return isNegative ? -result : result;
}

//
// Date and time operators
//

inline QDate operator+(const QDate &date, double x) { return date.addDays( convert<int>(x)); }
inline QDate operator-(const QDate &date, double x) { return date.addDays(-convert<int>(x)); }
inline int   operator-(const QDate &a, const QDate &b) { return numeric_cast<int>(b.daysTo(a)); }
inline QDate operator+(double x, const QDate &date) { return date + x; }

inline QTime  operator+(const QTime &time, double x) { return time.addMSecs( convert<int>(3600*1000*x)); }
inline QTime  operator-(const QTime &time, double x) { return time.addMSecs(-convert<int>(3600*1000*x)); }
inline double operator-(const QTime &a, const QTime &b) { return b.secsTo(a)/3600.; }
inline QTime  operator+(double x, const QTime &time) { return time + x; }

inline QDateTime operator+(const QDateTime &dt, int x) { return dt.addDays( x); }
inline QDateTime operator-(const QDateTime &dt, int x) { return dt.addDays(-x); }
inline double    operator-(const QDateTime &a, const QDateTime &b) { return b.secsTo(a)/3600./24.; }
inline QDateTime operator+(int x, const QDateTime &dt) { return dt + x; }

inline QDateTime operator+(const QDateTime &dt, double x) {
    QDateTime y(dt);
    return y.addDays(convert<int>(floor(x))).addSecs( convert<int>(24*3600*(x-floor(x))));
}
inline QDateTime operator-(const QDateTime &dt, double x) {
    QDateTime y(dt);
    return y.addDays(convert<int>(floor(x))).addSecs(-convert<int>(24*3600*(x-floor(x))));
}
inline QDateTime operator+(double x, const QDateTime &dt) { return dt + x; }

//inline BareDate operator+(const BareDate &date, double x) { return date.addBareDays( convert<int>(x)); }
//inline BareDate operator-(const BareDate &date, double x) { return date.addBareDays(-convert<int>(x)); }
//inline int      operator-(const BareDate &a, const BareDate &b) { return numeric_cast<int>(b.date().daysTo(a.date())); }
//inline BareDate operator+(double x, const BareDate &date) { return date + x; }

//
// Template functions for vector-scalar operations
//

template <class T, class U, class V> Value add_vector_scalar(U a, V b) {
    auto A = a.data();
    int n = a.size();
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) + b;
    return Value(result);
}

template <class T, class U, class V> Value subtract_vector_scalar(U a, V b) {
    auto A = a.data();
    int n = a.size();
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) - b;
    return Value(result);
}

template <class T, class U, class V> Value multiply_vector_scalar(U a, V b) {
    auto A = a.data();
    int n = a.size();
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) * b;
    return Value(result);
}

template <class T, class U, class V> Value divide_vector_scalar(U a, V b) {
    auto A = a.data();
    int n = a.size();
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) / b;
    return Value(result);
}

template <class T, class U, class V> Value exp_vector_scalar(U a, V b) {
    auto A = a.data();
    int n = a.size();
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = pow(*A++,  b);
    return Value(result);
}

template <class T> Value exp_vector_scalar(vint a, int b) {
    auto A = a.data();
    int n = a.size();
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = ipow(*A++,  b);
    return Value(result);
}

Value and_vector_scalar(vbool a, bool b) {
    auto A = a.data();
    int n = a.size();
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = *A++ && b;
    return Value(result);
}

Value or_vector_scalar(vbool a, bool b) {
    auto A = a.data();
    int n = a.size();
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = *A++ || b;
    return Value(result);
}

template <class U, class V> Value larger_vector_scalar(U a, V b) {
    auto A = a.data();
    int n = a.size();
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) > b;
    return Value(result);
}

template <class U, class V> Value larger_or_equal_vector_scalar(U a, V b) {
    auto A = a.data();
    int n = a.size();
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) >= b;
    return Value(result);
}

template <class U, class V> Value less_vector_scalar(U a, V b) {
    auto A = a.data();
    int n = a.size();
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) < b;
    return Value(result);
}

template <class U, class V> Value less_or_equal_vector_scalar(U a, V b) {
    auto A = a.data();
    int n = a.size();
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) <= b;
    return Value(result);
}

template <class U, class V> Value equal_vector_scalar(U a, V b) {
    auto A = a.data();
    int n = a.size();
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) == b;
    return Value(result);
}

template <class U, class V> Value not_equal_vector_scalar(U a, V b) {
    auto A = a.data();
    int n = a.size();
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) != b;
    return Value(result);
}

//
// Template functions for vector-vector operations
//

template <class T, class U, class V> Value add_vector_vector(U a, V b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) + (*B++);
    return Value(result);
}

template <class T, class U, class V> Value subtract_vector_vector(U a, V b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) - (*B++);
    return Value(result);
}

template <class T, class U, class V> Value multiply_vector_vector(U a, V b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) * (*B++);
    return Value(result);
}

template <class T, class U, class V> Value divide_vector_vector(U a, V b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = (*A++) / (*B++);
    return Value(result);
}

template <class T, class U, class V> Value exp_vector_vector(U a, V b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = pow(*A++,  *B++);
    return Value(result);
}

template <class T> Value exp_vector_vector(vint a, vint b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = ipow(*A++,  *B++);
    return Value(result);
}

Value and_vector_vector(vbool a, vbool b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = *A++ && *B++;
    return Value(result);
}

Value or_vector_vector(vbool a, vbool b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = *A++ || *B++;
    return Value(result);
}

template <class U, class V> Value larger_vector_vector(U a, V b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = *A++ > *B++;
    return Value(result);
}

template <class U, class V> Value larger_or_equal_vector_vector(U a, V b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = *A++ >= *B++;
    return Value(result);
}

template <class U, class V> Value less_vector_vector(U a, V b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = *A++ < *B++;
    return Value(result);
}

template <class U, class V> Value less_or_equal_vector_vector(U a, V b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = *A++ <= *B++;
    return Value(result);
}

template <class U, class V> Value equal_vector_vector(U a, V b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = *A++ == *B++;
    return Value(result);
}

template <class U, class V> Value not_equal_vector_vector(U a, V b) {
    auto A = a.data();
    auto B = b.data();
    int n = a.size();
    if (n != b.size())
        ThrowException("Vectors must have equal size").value1(n).value2(b.size());
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = *A++ != *B++;
    return Value(result);
}


Value not_vector(vbool a) {
    auto A = a.data();
    int n = a.size();
    vbool result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = !*A++;
    return Value(result);
}

template <class T> Value negate_vector(T a) {
    auto A = a.data();
    int n = a.size();
    T result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = -*A++;
    return Value(result);
}


#define SWITCH_A \
    switch (a.type()) { \
    case Type::Uninitialized : \
    case Type::Null    : return Value::null();

#define SWITCH_B \
    switch (b.type()) { \
    case Type::Uninitialized : \
    case Type::Null    : return Value::null();

//
// Addition
//

Value add(const Value &a, const Value &b) {
    using Type = Value::Type;

    if (!a.isVector() && b.isVector()) return add(b, a);

    SWITCH_A
    case Type::Bool:
        SWITCH_B
        case Type::String  : return Value(a.as<QString>() + b.as<QString>());
        default            : ;
        }
        break;
    case Type::Int:
        SWITCH_B
        case Type::Int     : return Value(a.as<int>() + b.as<int>());
        case Type::Double  : return Value(a.as<int>() + b.as<double>());
        case Type::String  : return Value(a.as<QString>() + b.as<QString>());
        case Type::Date    : return Value(a.as<int>() + b.as<QDate>());
        case Type::Time    : return Value(a.as<int>() + b.as<QTime>());
        case Type::DateTime: return Value(a.as<int>() + b.as<QDateTime>());
        case Type::BareDate: return Value(a.as<int>() + b.as<BareDate>());
        default            : ;
        }
        break;
    case Type::Double:
        SWITCH_B
        case Type::Int     : return Value(a.as<double>() + b.as<int>());
        case Type::Double  : return Value(a.as<double>() + b.as<double>());
        case Type::String  : return Value(a.as<QString>() + b.as<QString>());
        case Type::Date    : return Value(a.as<double>() + b.as<QDate>());
        case Type::Time    : return Value(a.as<double>() + b.as<QTime>());
        case Type::DateTime: return Value(a.as<double>() + b.as<QDateTime>());
        case Type::BareDate: return Value(a.as<double>() + b.as<BareDate>());
        default            : ;
        }
        break;
    case Type::String:
        return Value(a.as<QString>() + b.as<QString>());
        break;
    case Type::Date:
        SWITCH_B
        case Type::Int     : return Value(a.as<QDate>() + b.as<int>());
        case Type::Double  : return Value(a.as<QDate>() + b.as<double>());
        case Type::String  : return Value(a.as<QString>() + b.as<QString>());
        default            : ;
        }
        break;
    case Type::Time:
        SWITCH_B
        case Type::Int     : return Value(a.as<QTime>() + b.as<int>());
        case Type::Double  : return Value(a.as<QTime>() + b.as<double>());
        case Type::String  : return Value(a.as<QString>() + b.as<QString>());
        default            : ;
        }
        break;
    case Type::DateTime:
        SWITCH_B
        case Type::Int     : return Value(a.as<QDateTime>() + b.as<int>());
        case Type::Double  : return Value(a.as<QDateTime>() + b.as<double>());
        case Type::String  : return Value(a.as<QString>() + b.as<QString>());
        default            : ;
        }
        break;
    case Type::BareDate:
        SWITCH_B
        case Type::Int     : return Value(a.as<BareDate>() + b.as<int>());
        case Type::Double  : return Value(a.as<BareDate>() + b.as<double>());
        case Type::String  : return Value(a.as<QString>() + b.as<QString>());
        default            : ;
        }
        break;
    case Type::Path:
        break;
    //
    // Vector
    //
    case Type::VecBool:
        break;
    case Type::VecInt:
        SWITCH_B
        case Type::Int     : return add_vector_scalar<vint>      (a.as<vint>(), b.as<int>());
        case Type::Double  : return add_vector_scalar<vdouble>   (a.as<vint>(), b.as<double>());
        case Type::Date    : return add_vector_scalar<vQDate>    (a.as<vint>(), b.as<QDate>());
        case Type::Time    : return add_vector_scalar<vQTime>    (a.as<vint>(), b.as<QTime>());
        case Type::DateTime: return add_vector_scalar<vQDateTime>(a.as<vint>(), b.as<QDateTime>());
        case Type::BareDate: return add_vector_scalar<vBareDate> (a.as<vint>(), b.as<BareDate>());

        case Type::VecInt     : return add_vector_vector<vint>      (a.as<vint>(), b.as<vint>());
        case Type::VecDouble  : return add_vector_vector<vdouble>   (a.as<vint>(), b.as<vdouble>());
        case Type::VecDate    : return add_vector_vector<vQDate>    (a.as<vint>(), b.as<vQDate>());
        case Type::VecTime    : return add_vector_vector<vQTime>    (a.as<vint>(), b.as<vQTime>());
        case Type::VecDateTime: return add_vector_vector<vQDateTime>(a.as<vint>(), b.as<vQDateTime>());
        case Type::VecBareDate: return add_vector_vector<vBareDate> (a.as<vint>(), b.as<vBareDate>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        SWITCH_B
        case Type::Int     : return add_vector_scalar<vdouble>   (a.as<vdouble>(), b.as<int>());
        case Type::Double  : return add_vector_scalar<vdouble>   (a.as<vdouble>(), b.as<double>());
        case Type::Date    : return add_vector_scalar<vQDate>    (a.as<vdouble>(), b.as<QDate>());
        case Type::Time    : return add_vector_scalar<vQTime>    (a.as<vdouble>(), b.as<QTime>());
        case Type::DateTime: return add_vector_scalar<vQDateTime>(a.as<vdouble>(), b.as<QDateTime>());
        case Type::BareDate: return add_vector_scalar<vBareDate> (a.as<vdouble>(), b.as<BareDate>());

        case Type::VecInt     : return add_vector_vector<vdouble>   (a.as<vdouble>(), b.as<vint>());
        case Type::VecDouble  : return add_vector_vector<vdouble>   (a.as<vdouble>(), b.as<vdouble>());
        case Type::VecDate    : return add_vector_vector<vQDate>    (a.as<vdouble>(), b.as<vQDate>());
        case Type::VecTime    : return add_vector_vector<vQTime>    (a.as<vdouble>(), b.as<vQTime>());
        case Type::VecDateTime: return add_vector_vector<vQDateTime>(a.as<vdouble>(), b.as<vQDateTime>());
        case Type::VecBareDate: return add_vector_vector<vBareDate> (a.as<vdouble>(), b.as<vBareDate>());
        default               : ;
        }
        break;
    case Type::VecString:
        SWITCH_B
        case Type::String    : return add_vector_scalar<vQString> (a.as<vQString>(), b.as< QString>());
        case Type::VecString : return add_vector_vector<vQString> (a.as<vQString>(), b.as<vQString>());
        default              : ;
        }
        break;
    case Type::VecDate:
        SWITCH_B
        case Type::Int       : return add_vector_scalar<vQDate> (a.as<vQDate>(), b.as<int>());
        case Type::Double    : return add_vector_scalar<vQDate> (a.as<vQDate>(), b.as<double>());
        case Type::VecInt    : return add_vector_vector<vQDate> (a.as<vQDate>(), b.as<vint>());
        case Type::VecDouble : return add_vector_vector<vQDate> (a.as<vQDate>(), b.as<vdouble>());
        default              : ;
        }
        break;
    case Type::VecTime:
        SWITCH_B
        case Type::Int       : return add_vector_scalar<vQTime> (a.as<vQTime>(), b.as<int>());
        case Type::Double    : return add_vector_scalar<vQTime> (a.as<vQTime>(), b.as<double>());
        case Type::VecInt    : return add_vector_vector<vQTime> (a.as<vQTime>(), b.as<vint>());
        case Type::VecDouble : return add_vector_vector<vQTime> (a.as<vQTime>(), b.as<vdouble>());
        default            : ;
        }
        break;
    case Type::VecDateTime:
        SWITCH_B
        case Type::Int       : return add_vector_scalar<vQDateTime> (a.as<vQDateTime>(), b.as<int>());
        case Type::Double    : return add_vector_scalar<vQDateTime> (a.as<vQDateTime>(), b.as<double>());
        case Type::VecInt    : return add_vector_vector<vQDateTime> (a.as<vQDateTime>(), b.as<vint>());
        case Type::VecDouble : return add_vector_vector<vQDateTime> (a.as<vQDateTime>(), b.as<vdouble>());
        default              : ;
        }
        break;
    case Type::VecBareDate:
        SWITCH_B
        case Type::Int       : return add_vector_scalar<vBareDate> (a.as<vBareDate>(), b.as<int>());
        case Type::Double    : return add_vector_scalar<vBareDate> (a.as<vBareDate>(), b.as<double>());
        case Type::VecInt    : return add_vector_vector<vBareDate> (a.as<vBareDate>(), b.as<vint>());
        case Type::VecDouble : return add_vector_vector<vBareDate> (a.as<vBareDate>(), b.as<vdouble>());
        default              : ;
        }
        break;
    }
    ThrowException("Incompatible operands in addition").value(a.typeName() +" + "+ b.typeName());
    return Value();
}

//
// Subtraction
//

Value subtract(const Value &a, const Value &b) {
    using Type = Value::Type;

    if (!a.isVector() && b.isVector())
        ThrowException("Vector (v) must preceed scalar (s) in subtraction: v-s not s-v")
                .value(a.typeName() +"-"+ (b.typeName()));

    SWITCH_A
    case Type::Bool:
        break;
    case Type::Int:
        SWITCH_B
        case Type::Int     : return Value(a.as<int>() - b.as<int>());
        case Type::Double  : return Value(a.as<int>() - b.as<double>());
        default            : ;
        }
        break;
    case Type::Double:
        SWITCH_B
        case Type::Int     : return Value(a.as<double>() - b.as<int>());
        case Type::Double  : return Value(a.as<double>() - b.as<double>());
        default            : ;
        }
        break;
    case Type::String:
        break;
    case Type::Date:
        SWITCH_B
        case Type::Int     : return Value(a.as<QDate>() - b.as<int>());
        case Type::Double  : return Value(a.as<QDate>() - b.as<double>());
        case Type::Date    : return Value(a.as<QDate>() - b.as<QDate>());
        default            : ;
        }
        break;
    case Type::Time:
        SWITCH_B
        case Type::Int     : return Value(a.as<QTime>() - b.as<int>());
        case Type::Double  : return Value(a.as<QTime>() - b.as<double>());
        case Type::Time    : return Value(a.as<QTime>() - b.as<QTime>());
        default            : ;
        }
        break;
    case Type::DateTime:
        SWITCH_B
        case Type::Int     : return Value(a.as<QDateTime>() - b.as<int>());
        case Type::Double  : return Value(a.as<QDateTime>() - b.as<double>());
        case Type::DateTime: return Value(a.as<QDateTime>() - b.as<QDateTime>());
        default            : ;
        }
        break;
    case Type::BareDate:
        SWITCH_B
        case Type::Int     : return Value(a.as<BareDate>() - b.as<int>());
        case Type::Double  : return Value(a.as<BareDate>() - b.as<double>());
        case Type::Date    : return Value(a.as<BareDate>() - b.as<BareDate>());
        default            : ;
        }
        break;
    case Type::Path:
        break;
    //
    // Vector operations
    //
    case Type::VecBool:
        break;
    case Type::VecInt:
        SWITCH_B
        case Type::Int        : return subtract_vector_scalar<vint>    (a.as<vint>(), b.as<int>());
        case Type::Double     : return subtract_vector_scalar<vdouble> (a.as<vint>(), b.as<double>());
        case Type::VecInt     : return subtract_vector_vector<vint>    (a.as<vint>(), b.as<vint>());
        case Type::VecDouble  : return subtract_vector_vector<vdouble> (a.as<vint>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        SWITCH_B
        case Type::Int        : return subtract_vector_scalar<vdouble> (a.as<vdouble>(), b.as<int>());
        case Type::Double     : return subtract_vector_scalar<vdouble> (a.as<vdouble>(), b.as<double>());
        case Type::VecInt     : return subtract_vector_vector<vdouble> (a.as<vdouble>(), b.as<vint>());
        case Type::VecDouble  : return subtract_vector_vector<vdouble> (a.as<vdouble>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecString:
        break;
    case Type::VecDate:
        SWITCH_B
        case Type::Int       : return subtract_vector_scalar<vQDate> (a.as<vQDate>(), b.as<int>());
        case Type::Double    : return subtract_vector_scalar<vQDate> (a.as<vQDate>(), b.as<double>());
        case Type::VecInt    : return subtract_vector_vector<vQDate> (a.as<vQDate>(), b.as<vint>());
        case Type::VecDouble : return subtract_vector_vector<vQDate> (a.as<vQDate>(), b.as<vdouble>());
        default              : ;
        }
        break;
    case Type::VecTime:
        SWITCH_B
        case Type::Int       : return subtract_vector_scalar<vQTime> (a.as<vQTime>(), b.as<int>());
        case Type::Double    : return subtract_vector_scalar<vQTime> (a.as<vQTime>(), b.as<double>());
        case Type::VecInt    : return subtract_vector_vector<vQTime> (a.as<vQTime>(), b.as<vint>());
        case Type::VecDouble : return subtract_vector_vector<vQTime> (a.as<vQTime>(), b.as<vdouble>());
        default            : ;
        }
        break;
    case Type::VecDateTime:
        SWITCH_B
        case Type::Int       : return subtract_vector_scalar<vQDateTime> (a.as<vQDateTime>(), b.as<int>());
        case Type::Double    : return subtract_vector_scalar<vQDateTime> (a.as<vQDateTime>(), b.as<double>());
        case Type::VecInt    : return subtract_vector_vector<vQDateTime> (a.as<vQDateTime>(), b.as<vint>());
        case Type::VecDouble : return subtract_vector_vector<vQDateTime> (a.as<vQDateTime>(), b.as<vdouble>());
        default              : ;
        }
        break;
    case Type::VecBareDate:
        SWITCH_B
        case Type::Int       : return subtract_vector_scalar<vBareDate> (a.as<vBareDate>(), b.as<int>());
        case Type::Double    : return subtract_vector_scalar<vBareDate> (a.as<vBareDate>(), b.as<double>());
        case Type::VecInt    : return subtract_vector_vector<vBareDate> (a.as<vBareDate>(), b.as<vint>());
        case Type::VecDouble : return subtract_vector_vector<vBareDate> (a.as<vBareDate>(), b.as<vdouble>());
        default              : ;
        }
        break;
    }
    ThrowException("Incompatible operands in subtraction").value(a.typeName() +" - "+ b.typeName());
    return Value();
}


//
// Multiplication
//

Value multiply(const Value &a, const Value &b) {
    using Type = Value::Type;

    if (!a.isVector() && b.isVector()) return multiply(b, a);

    SWITCH_A
    case Type::Int:
        SWITCH_B
        case Type::Int     : return Value(a.as<int>() * b.as<int>());
        case Type::Double  : return Value(a.as<int>() * b.as<double>());
        default            : ;
        }
        break;
    case Type::Double:
        SWITCH_B
        case Type::Int     : return Value(a.as<double>() * b.as<int>());
        case Type::Double  : return Value(a.as<double>() * b.as<double>());
        default            : ;
        }
        break;
    case Type::VecInt:
        SWITCH_B
        case Type::Int        : return multiply_vector_scalar<vint>      (a.as<vint>(), b.as<int>());
        case Type::Double     : return multiply_vector_scalar<vdouble>   (a.as<vint>(), b.as<double>());
        case Type::VecInt     : return multiply_vector_vector<vint>      (a.as<vint>(), b.as<vint>());
        case Type::VecDouble  : return multiply_vector_vector<vdouble>   (a.as<vint>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        SWITCH_B
        case Type::Int        : return multiply_vector_scalar<vdouble>   (a.as<vdouble>(), b.as<int>());
        case Type::Double     : return multiply_vector_scalar<vdouble>   (a.as<vdouble>(), b.as<double>());
        case Type::VecInt     : return multiply_vector_vector<vdouble>   (a.as<vdouble>(), b.as<vint>());
        case Type::VecDouble  : return multiply_vector_vector<vdouble>   (a.as<vdouble>(), b.as<vdouble>());
        default               : ;
        }
        break;
    default: ;
    }
    ThrowException("Incompatible operands in multiplication").value(a.typeName() +" * "+ b.typeName());
    return Value();
}

//
// Division
//

Value divide(const Value &a, const Value &b) {
    using Type = Value::Type;

    if (!a.isVector() && b.isVector())
        ThrowException("Vector (v) must preceed scalar (s) in division: v/s not s/v")
            .value(a.typeName() +"/"+ (b.typeName()));

    SWITCH_A
    case Type::Int:
        SWITCH_B
        case Type::Int     : return Value(a.as<int>() / b.as<int>());
        case Type::Double  : return Value(a.as<int>() / b.as<double>());
        default            : ;
        }
        break;
    case Type::Double:
        SWITCH_B
        case Type::Int     : return Value(a.as<double>() / b.as<int>());
        case Type::Double  : return Value(a.as<double>() / b.as<double>());
        default            : ;
        }
        break;
    case Type::VecInt:
        SWITCH_B
        case Type::Int        : return divide_vector_scalar<vint>      (a.as<vint>(), b.as<int>());
        case Type::Double     : return divide_vector_scalar<vdouble>   (a.as<vint>(), b.as<double>());
        case Type::VecInt     : return divide_vector_vector<vint>      (a.as<vint>(), b.as<vint>());
        case Type::VecDouble  : return divide_vector_vector<vdouble>   (a.as<vint>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        SWITCH_B
        case Type::Int        : return divide_vector_scalar<vdouble>   (a.as<vdouble>(), b.as<int>());
        case Type::Double     : return divide_vector_scalar<vdouble>   (a.as<vdouble>(), b.as<double>());
        case Type::VecInt     : return divide_vector_vector<vdouble>   (a.as<vdouble>(), b.as<vint>());
        case Type::VecDouble  : return divide_vector_vector<vdouble>   (a.as<vdouble>(), b.as<vdouble>());
        default               : ;
        }
        break;
    default: ;
    }
    ThrowException("Incompatible operands in division").value(a.typeName() +" / "+ b.typeName());
    return Value();
}

//
// Exponentiation
//

Value exponentiate(const Value &a, const Value &b) {
    using Type = Value::Type;
    using std::pow;

    if (!a.isVector() && b.isVector())
        ThrowException("Vector (v) must preceed scalar (s) in exponentiation: v^s not s^v")
            .value(a.typeName() +"/"+ (b.typeName()));

    SWITCH_A
    case Type::Int:
        SWITCH_B
        case Type::Int     : return Value(ipow(a.as<int>(), b.as<int>()));
        case Type::Double  : return Value( pow(a.as<int>(), b.as<double>()));
        default            : ;
        }
        break;
    case Type::Double:
        SWITCH_B
        case Type::Int     : return Value(pow(a.as<double>(), b.as<int>()));
        case Type::Double  : return Value(pow(a.as<double>(), b.as<double>()));
        default            : ;
        }
        break;
    case Type::VecInt:
        SWITCH_B
        case Type::Int        : return exp_vector_scalar<vint>    (a.as<vint>(), b.as<int>());
        case Type::Double     : return exp_vector_scalar<vdouble> (a.as<vint>(), b.as<double>());
        case Type::VecInt     : return exp_vector_vector<vint>    (a.as<vint>(), b.as<vint>());
        case Type::VecDouble  : return exp_vector_vector<vdouble> (a.as<vint>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        SWITCH_B
        case Type::Int        : return exp_vector_scalar<vdouble> (a.as<vdouble>(), b.as<int>());
        case Type::Double     : return exp_vector_scalar<vdouble> (a.as<vdouble>(), b.as<double>());
        case Type::VecInt     : return exp_vector_vector<vdouble> (a.as<vdouble>(), b.as<vint>());
        case Type::VecDouble  : return exp_vector_vector<vdouble> (a.as<vdouble>(), b.as<vdouble>());
        default               : ;
        }
        break;
    default: ;
    }
    ThrowException("Incompatible operands in division").value(a.typeName() +" / "+ b.typeName());
    return Value();
}

//
// Larger than
//

Value larger(const Value &a, const Value &b) {
    using Type = Value::Type;

    if (!a.isVector() && b.isVector()) return larger(b, a);

    SWITCH_A
    case Type::Bool:
        break;
    case Type::Int:
        SWITCH_B
        case Type::Double  : return Value(a.as<int>() > b.as<double>());
        case Type::Int     : return Value(a.as<int>() > b.as<int>());
        default            : ;
        }
        break;
    case Type::Double:
        SWITCH_B
        case Type::Int     : return Value(a.as<double>() > b.as<int>());
        case Type::Double  : return Value(a.as<double>() > b.as<double>());
        default            : ;
        }
        break;
    case Type::String:
        SWITCH_B
        case Type::String  : return Value(a.as<QString>() > b.as<QString>());
        default            : ;
        }
        break;
    case Type::Date:
        SWITCH_B
        case Type::Date    : return Value(a.as<QDate>()    > b.as<QDate>());
        case Type::BareDate: return Value(a.as<BareDate>() > b.as<BareDate>());
        default            : ;
        }
        break;
    case Type::Time:
        SWITCH_B
        case Type::Time    : return Value(a.as<QTime>() > b.as<QTime>());
        default            : ;
        }
        break;
    case Type::DateTime:
        SWITCH_B
        case Type::DateTime: return Value(a.as<QDateTime>() > b.as<QDateTime>());
        default            : ;
        }
        break;
    case Type::BareDate:
        SWITCH_B
        case Type::Date    :
        case Type::BareDate: return Value(a.as<BareDate>() > b.as<BareDate>());
        default            : ;
        }
        break;
    case Type::Path:
        break;
    //
    // Vector
    //
    case Type::VecBool:
        break;
    case Type::VecInt:
        SWITCH_B
        case Type::Int        : return larger_vector_scalar(a.as<vint>(), b.as<int>());
        case Type::Double     : return larger_vector_scalar(a.as<vint>(), b.as<double>());
        case Type::VecInt     : return larger_vector_vector(a.as<vint>(), b.as<vint>());
        case Type::VecDouble  : return larger_vector_vector(a.as<vint>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        SWITCH_B
        case Type::Int        : return larger_vector_scalar(a.as<vdouble>(), b.as<int>());
        case Type::Double     : return larger_vector_scalar(a.as<vdouble>(), b.as<double>());
        case Type::VecInt     : return larger_vector_vector(a.as<vdouble>(), b.as<vint>());
        case Type::VecDouble  : return larger_vector_vector(a.as<vdouble>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecString:
        SWITCH_B
        case Type::String     : return larger_vector_scalar(a.as<vQString>(), b.as<QString>());
        case Type::VecString  : return larger_vector_vector(a.as<vQString>(), b.as<vQString>());
        default               : ;
        }
        break;
    case Type::VecDate:
        SWITCH_B
        case Type::Date       : return larger_vector_scalar(a.as<vQDate>(), b.as< QDate>());
        case Type::VecDate    : return larger_vector_vector(a.as<vQDate>(), b.as<vQDate>());
        default               : ;
        }
        break;
    case Type::VecTime:
        SWITCH_B
        case Type::Time       : return larger_vector_scalar(a.as<vQTime>(), b.as< QTime>());
        case Type::VecTime    : return larger_vector_vector(a.as<vQTime>(), b.as<vQTime>());
        default               : ;
        }
        break;
    case Type::VecDateTime:
        SWITCH_B
        case Type::DateTime   : return larger_vector_scalar(a.as<vQDateTime>(), b.as< QDateTime>());
        case Type::VecDateTime: return larger_vector_vector(a.as<vQDateTime>(), b.as<vQDateTime>());
        default               : ;
        }
        break;
    case Type::VecBareDate:
        SWITCH_B
        case Type::BareDate   : return larger_vector_scalar(a.as<vBareDate>(), b.as< BareDate>());
        case Type::VecBareDate: return larger_vector_vector(a.as<vBareDate>(), b.as<vBareDate>());
        default               : ;
        }
        break;
    }
    ThrowException("Incompatible operands in comparison").value(a.typeName() +" > "+ b.typeName());
    return Value();
}

//
// Larger than or equal to
//

Value largerOrEqual(const Value &a, const Value &b) {
    using Type = Value::Type;

    if (!a.isVector() && b.isVector()) return largerOrEqual(b, a);

    SWITCH_A
    case Type::Bool:
        break;
    case Type::Int:
        SWITCH_B
        case Type::Double  : return Value(a.as<int>() >= b.as<double>());
        case Type::Int     : return Value(a.as<int>() >= b.as<int>());
        default            : ;
        }
        break;
    case Type::Double:
        SWITCH_B
        case Type::Int     : return Value(a.as<double>() >= b.as<int>());
        case Type::Double  : return Value(a.as<double>() >= b.as<double>());
        default            : ;
        }
        break;
    case Type::String:
        SWITCH_B
        case Type::String  : return Value(a.as<QString>() >= b.as<QString>());
        default            : ;
        }
        break;
    case Type::Date:
        SWITCH_B
        case Type::Date    : return Value(a.as<QDate>()    >= b.as<QDate>());
        case Type::BareDate: return Value(a.as<BareDate>() >= b.as<BareDate>());
        default            : ;
        }
        break;
    case Type::Time:
        SWITCH_B
        case Type::Time    : return Value(a.as<QTime>() >= b.as<QTime>());
        default            : ;
        }
        break;
    case Type::DateTime:
        SWITCH_B
        case Type::DateTime: return Value(a.as<QDateTime>() >= b.as<QDateTime>());
        default            : ;
        }
        break;
    case Type::BareDate:
        SWITCH_B
        case Type::Date    :
        case Type::BareDate: return Value(a.as<BareDate>() >= b.as<BareDate>());
        default            : ;
        }
        break;
    case Type::Path:
        break;
    //
    // Vector
    //
    case Type::VecBool:
        break;
    case Type::VecInt:
        SWITCH_B
        case Type::Int        : return larger_or_equal_vector_scalar(a.as<vint>(), b.as<int>());
        case Type::Double     : return larger_or_equal_vector_scalar(a.as<vint>(), b.as<double>());
        case Type::VecInt     : return larger_or_equal_vector_vector(a.as<vint>(), b.as<vint>());
        case Type::VecDouble  : return larger_or_equal_vector_vector(a.as<vint>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        SWITCH_B
        case Type::Int        : return larger_or_equal_vector_scalar(a.as<vdouble>(), b.as<int>());
        case Type::Double     : return larger_or_equal_vector_scalar(a.as<vdouble>(), b.as<double>());
        case Type::VecInt     : return larger_or_equal_vector_vector(a.as<vdouble>(), b.as<vint>());
        case Type::VecDouble  : return larger_or_equal_vector_vector(a.as<vdouble>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecString:
        SWITCH_B
        case Type::String     : return larger_or_equal_vector_scalar(a.as<vQString>(), b.as<QString>());
        case Type::VecString  : return larger_or_equal_vector_vector(a.as<vQString>(), b.as<vQString>());
        default               : ;
        }
        break;
    case Type::VecDate:
        SWITCH_B
        case Type::Date       : return larger_or_equal_vector_scalar(a.as<vQDate>(), b.as< QDate>());
        case Type::VecDate    : return larger_or_equal_vector_vector(a.as<vQDate>(), b.as<vQDate>());
        default               : ;
        }
        break;
    case Type::VecTime:
        SWITCH_B
        case Type::Time       : return larger_or_equal_vector_scalar(a.as<vQTime>(), b.as< QTime>());
        case Type::VecTime    : return larger_or_equal_vector_vector(a.as<vQTime>(), b.as<vQTime>());
        default               : ;
        }
        break;
    case Type::VecDateTime:
        SWITCH_B
        case Type::DateTime   : return larger_or_equal_vector_scalar(a.as<vQDateTime>(), b.as< QDateTime>());
        case Type::VecDateTime: return larger_or_equal_vector_vector(a.as<vQDateTime>(), b.as<vQDateTime>());
        default               : ;
        }
        break;
    case Type::VecBareDate:
        SWITCH_B
        case Type::BareDate   : return larger_or_equal_vector_scalar(a.as<vBareDate>(), b.as< BareDate>());
        case Type::VecBareDate: return larger_or_equal_vector_vector(a.as<vBareDate>(), b.as<vBareDate>());
        default               : ;
        }
        break;
    }
    ThrowException("Incompatible operands in comparison").value(a.typeName() +" >= "+ b.typeName());
    return Value();
}

//
// Less than
//

Value less(const Value &a, const Value &b) {
    using Type = Value::Type;

    if (!a.isVector() && b.isVector()) return less(b, a);

    SWITCH_A
    case Type::Bool:
        break;
    case Type::Int:
        SWITCH_B
        case Type::Double  : return Value(a.as<int>() < b.as<double>());
        case Type::Int     : return Value(a.as<int>() < b.as<int>());
        default            : ;
        }
        break;
    case Type::Double:
        SWITCH_B
        case Type::Int     : return Value(a.as<double>() < b.as<int>());
        case Type::Double  : return Value(a.as<double>() < b.as<double>());
        default            : ;
        }
        break;
    case Type::String:
        SWITCH_B
        case Type::String  : return Value(a.as<QString>() < b.as<QString>());
        default            : ;
        }
        break;
    case Type::Date:
        SWITCH_B
        case Type::Date    : return Value(a.as<QDate>()    < b.as<QDate>());
        case Type::BareDate: return Value(a.as<BareDate>() < b.as<BareDate>());
        default            : ;
        }
        break;
    case Type::Time:
        SWITCH_B
        case Type::Time    : return Value(a.as<QTime>() < b.as<QTime>());
        default            : ;
        }
        break;
    case Type::DateTime:
        SWITCH_B
        case Type::DateTime: return Value(a.as<QDateTime>() < b.as<QDateTime>());
        default            : ;
        }
        break;
    case Type::BareDate:
        SWITCH_B
        case Type::Date    :
        case Type::BareDate: return Value(a.as<BareDate>() < b.as<BareDate>());
        default            : ;
        }
        break;
    case Type::Path:
        break;
    //
    // Vector
    //
    case Type::VecBool:
        break;
    case Type::VecInt:
        SWITCH_B
        case Type::Int        : return less_vector_scalar(a.as<vint>(), b.as<int>());
        case Type::Double     : return less_vector_scalar(a.as<vint>(), b.as<double>());
        case Type::VecInt     : return less_vector_vector(a.as<vint>(), b.as<vint>());
        case Type::VecDouble  : return less_vector_vector(a.as<vint>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        SWITCH_B
        case Type::Int        : return less_vector_scalar(a.as<vdouble>(), b.as<int>());
        case Type::Double     : return less_vector_scalar(a.as<vdouble>(), b.as<double>());
        case Type::VecInt     : return less_vector_vector(a.as<vdouble>(), b.as<vint>());
        case Type::VecDouble  : return less_vector_vector(a.as<vdouble>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecString:
        SWITCH_B
        case Type::String     : return less_vector_scalar(a.as<vQString>(), b.as<QString>());
        case Type::VecString  : return less_vector_vector(a.as<vQString>(), b.as<vQString>());
        default               : ;
        }
        break;
    case Type::VecDate:
        SWITCH_B
        case Type::Date       : return less_vector_scalar(a.as<vQDate>(), b.as< QDate>());
        case Type::VecDate    : return less_vector_vector(a.as<vQDate>(), b.as<vQDate>());
        default               : ;
        }
        break;
    case Type::VecTime:
        SWITCH_B
        case Type::Time       : return less_vector_scalar(a.as<vQTime>(), b.as< QTime>());
        case Type::VecTime    : return less_vector_vector(a.as<vQTime>(), b.as<vQTime>());
        default               : ;
        }
        break;
    case Type::VecDateTime:
        SWITCH_B
        case Type::DateTime   : return less_vector_scalar(a.as<vQDateTime>(), b.as< QDateTime>());
        case Type::VecDateTime: return less_vector_vector(a.as<vQDateTime>(), b.as<vQDateTime>());
        default               : ;
        }
        break;
    case Type::VecBareDate:
        SWITCH_B
        case Type::BareDate   : return less_vector_scalar(a.as<vBareDate>(), b.as< BareDate>());
        case Type::VecBareDate: return less_vector_vector(a.as<vBareDate>(), b.as<vBareDate>());
        default               : ;
        }
        break;
    }
    ThrowException("Incompatible operands in comparison").value(a.typeName() +" < "+ b.typeName());
    return Value();
}

//
// Less than or equal to
//

Value lessOrEqual(const Value &a, const Value &b) {
    using Type = Value::Type;

    if (!a.isVector() && b.isVector()) return lessOrEqual(b, a);

    SWITCH_A
    case Type::Bool:
        break;
    case Type::Int:
        SWITCH_B
        case Type::Double  : return Value(a.as<int>() <= b.as<double>());
        case Type::Int     : return Value(a.as<int>() <= b.as<int>());
        default            : ;
        }
        break;
    case Type::Double:
        SWITCH_B
        case Type::Int     : return Value(a.as<double>() <= b.as<int>());
        case Type::Double  : return Value(a.as<double>() <= b.as<double>());
        default            : ;
        }
        break;
    case Type::String:
        SWITCH_B
        case Type::String  : return Value(a.as<QString>() <= b.as<QString>());
        default            : ;
        }
        break;
    case Type::Date:
        SWITCH_B
        case Type::Date    : return Value(a.as<QDate>()    <= b.as<QDate>());
        case Type::BareDate: return Value(a.as<BareDate>() <= b.as<BareDate>());
        default            : ;
        }
        break;
    case Type::Time:
        SWITCH_B
        case Type::Time    : return Value(a.as<QTime>() <= b.as<QTime>());
        default            : ;
        }
        break;
    case Type::DateTime:
        SWITCH_B
        case Type::DateTime: return Value(a.as<QDateTime>() <= b.as<QDateTime>());
        default            : ;
        }
        break;
    case Type::BareDate:
        SWITCH_B
        case Type::Date    :
        case Type::BareDate: return Value(a.as<BareDate>() <= b.as<BareDate>());
        default            : ;
        }
        break;
    case Type::Path:
        break;
    //
    // Vector
    //
    case Type::VecBool:
        break;
    case Type::VecInt:
        SWITCH_B
        case Type::Int        : return less_or_equal_vector_scalar(a.as<vint>(), b.as<int>());
        case Type::Double     : return less_or_equal_vector_scalar(a.as<vint>(), b.as<double>());
        case Type::VecInt     : return less_or_equal_vector_vector(a.as<vint>(), b.as<vint>());
        case Type::VecDouble  : return less_or_equal_vector_vector(a.as<vint>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        SWITCH_B
        case Type::Int        : return less_or_equal_vector_scalar(a.as<vdouble>(), b.as<int>());
        case Type::Double     : return less_or_equal_vector_scalar(a.as<vdouble>(), b.as<double>());
        case Type::VecInt     : return less_or_equal_vector_vector(a.as<vdouble>(), b.as<vint>());
        case Type::VecDouble  : return less_or_equal_vector_vector(a.as<vdouble>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecString:
        SWITCH_B
        case Type::String     : return less_or_equal_vector_scalar(a.as<vQString>(), b.as<QString>());
        case Type::VecString  : return less_or_equal_vector_vector(a.as<vQString>(), b.as<vQString>());
        default               : ;
        }
        break;
    case Type::VecDate:
        SWITCH_B
        case Type::Date       : return less_or_equal_vector_scalar(a.as<vQDate>(), b.as< QDate>());
        case Type::VecDate    : return less_or_equal_vector_vector(a.as<vQDate>(), b.as<vQDate>());
        default               : ;
        }
        break;
    case Type::VecTime:
        SWITCH_B
        case Type::Time       : return less_or_equal_vector_scalar(a.as<vQTime>(), b.as< QTime>());
        case Type::VecTime    : return less_or_equal_vector_vector(a.as<vQTime>(), b.as<vQTime>());
        default               : ;
        }
        break;
    case Type::VecDateTime:
        SWITCH_B
        case Type::DateTime   : return less_or_equal_vector_scalar(a.as<vQDateTime>(), b.as< QDateTime>());
        case Type::VecDateTime: return less_or_equal_vector_vector(a.as<vQDateTime>(), b.as<vQDateTime>());
        default               : ;
        }
        break;
    case Type::VecBareDate:
        SWITCH_B
        case Type::BareDate   : return less_or_equal_vector_scalar(a.as<vBareDate>(), b.as< BareDate>());
        case Type::VecBareDate: return less_or_equal_vector_vector(a.as<vBareDate>(), b.as<vBareDate>());
        default               : ;
        }
        break;
    }
    ThrowException("Incompatible operands in comparison").value(a.typeName() +" <= "+ b.typeName());
    return Value();
}

//
// Equal to
//

Value equal(const Value &a, const Value &b) {
    using Type = Value::Type;

    if (!a.isVector() && b.isVector()) return equal(b, a);

    SWITCH_A
    case Type::Bool:
        break;
    case Type::Int:
        SWITCH_B
        case Type::Double  : return Value(a.as<int>() == b.as<double>());
        case Type::Int     : return Value(a.as<int>() == b.as<int>());
        default            : ;
        }
        break;
    case Type::Double:
        SWITCH_B
        case Type::Int     : return Value(a.as<double>() == b.as<int>());
        case Type::Double  : return Value(a.as<double>() == b.as<double>());
        default            : ;
        }
        break;
    case Type::String:
        SWITCH_B
        case Type::String  : return Value(a.as<QString>() == b.as<QString>());
        default            : ;
        }
        break;
    case Type::Date:
        SWITCH_B
        case Type::Date    : return Value(a.as<QDate>()    == b.as<QDate>());
        case Type::BareDate: return Value(a.as<BareDate>() == b.as<BareDate>());
        default            : ;
        }
        break;
    case Type::Time:
        SWITCH_B
        case Type::Time    : return Value(a.as<QTime>() == b.as<QTime>());
        default            : ;
        }
        break;
    case Type::DateTime:
        SWITCH_B
        case Type::DateTime: return Value(a.as<QDateTime>() == b.as<QDateTime>());
        default            : ;
        }
        break;
    case Type::BareDate:
        SWITCH_B
        case Type::Date    :
        case Type::BareDate: return Value(a.as<BareDate>() == b.as<BareDate>());
        default            : ;
        }
        break;
    case Type::Path:
        break;
    //
    // Vector
    //
    case Type::VecBool:
        break;
    case Type::VecInt:
        SWITCH_B
        case Type::Int        : return equal_vector_scalar(a.as<vint>(), b.as<int>());
        case Type::Double     : return equal_vector_scalar(a.as<vint>(), b.as<double>());
        case Type::VecInt     : return equal_vector_vector(a.as<vint>(), b.as<vint>());
        case Type::VecDouble  : return equal_vector_vector(a.as<vint>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        SWITCH_B
        case Type::Int        : return equal_vector_scalar(a.as<vdouble>(), b.as<int>());
        case Type::Double     : return equal_vector_scalar(a.as<vdouble>(), b.as<double>());
        case Type::VecInt     : return equal_vector_vector(a.as<vdouble>(), b.as<vint>());
        case Type::VecDouble  : return equal_vector_vector(a.as<vdouble>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecString:
        SWITCH_B
        case Type::String     : return equal_vector_scalar(a.as<vQString>(), b.as<QString>());
        case Type::VecString  : return equal_vector_vector(a.as<vQString>(), b.as<vQString>());
        default               : ;
        }
        break;
    case Type::VecDate:
        SWITCH_B
        case Type::Date       : return equal_vector_scalar(a.as<vQDate>(), b.as< QDate>());
        case Type::VecDate    : return equal_vector_vector(a.as<vQDate>(), b.as<vQDate>());
        default               : ;
        }
        break;
    case Type::VecTime:
        SWITCH_B
        case Type::Time       : return equal_vector_scalar(a.as<vQTime>(), b.as< QTime>());
        case Type::VecTime    : return equal_vector_vector(a.as<vQTime>(), b.as<vQTime>());
        default               : ;
        }
        break;
    case Type::VecDateTime:
        SWITCH_B
        case Type::DateTime   : return equal_vector_scalar(a.as<vQDateTime>(), b.as< QDateTime>());
        case Type::VecDateTime: return equal_vector_vector(a.as<vQDateTime>(), b.as<vQDateTime>());
        default               : ;
        }
        break;
    case Type::VecBareDate:
        SWITCH_B
        case Type::BareDate   : return equal_vector_scalar(a.as<vBareDate>(), b.as< BareDate>());
        case Type::VecBareDate: return equal_vector_vector(a.as<vBareDate>(), b.as<vBareDate>());
        default               : ;
        }
        break;
    }
    ThrowException("Incompatible operands in comparison").value(a.typeName() +" == "+ b.typeName());
    return Value();
}

//
// Not equal to
//

Value notEqual(const Value &a, const Value &b) {
    using Type = Value::Type;

    if (!a.isVector() && b.isVector()) return notEqual(b, a);

    SWITCH_A
    case Type::Bool:
        break;
    case Type::Int:
        SWITCH_B
        case Type::Double  : return Value(a.as<int>() != b.as<double>());
        case Type::Int     : return Value(a.as<int>() != b.as<int>());
        default            : ;
        }
        break;
    case Type::Double:
        SWITCH_B
        case Type::Int     : return Value(a.as<double>() != b.as<int>());
        case Type::Double  : return Value(a.as<double>() != b.as<double>());
        default            : ;
        }
        break;
    case Type::String:
        SWITCH_B
        case Type::String  : return Value(a.as<QString>() != b.as<QString>());
        default            : ;
        }
        break;
    case Type::Date:
        SWITCH_B
        case Type::Date    : return Value(a.as<QDate>()    != b.as<QDate>());
        case Type::BareDate: return Value(a.as<BareDate>() != b.as<BareDate>());
        default            : ;
        }
        break;
    case Type::Time:
        SWITCH_B
        case Type::Time    : return Value(a.as<QTime>() != b.as<QTime>());
        default            : ;
        }
        break;
    case Type::DateTime:
        SWITCH_B
        case Type::DateTime: return Value(a.as<QDateTime>() != b.as<QDateTime>());
        default            : ;
        }
        break;
    case Type::BareDate:
        SWITCH_B
        case Type::Date    :
        case Type::BareDate: return Value(a.as<BareDate>() != b.as<BareDate>());
        default            : ;
        }
        break;
    case Type::Path:
        break;
    //
    // Vector
    //
    case Type::VecBool:
        break;
    case Type::VecInt:
        SWITCH_B
        case Type::Int        : return not_equal_vector_scalar(a.as<vint>(), b.as<int>());
        case Type::Double     : return not_equal_vector_scalar(a.as<vint>(), b.as<double>());
        case Type::VecInt     : return not_equal_vector_vector(a.as<vint>(), b.as<vint>());
        case Type::VecDouble  : return not_equal_vector_vector(a.as<vint>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecDouble:
        SWITCH_B
        case Type::Int        : return not_equal_vector_scalar(a.as<vdouble>(), b.as<int>());
        case Type::Double     : return not_equal_vector_scalar(a.as<vdouble>(), b.as<double>());
        case Type::VecInt     : return not_equal_vector_vector(a.as<vdouble>(), b.as<vint>());
        case Type::VecDouble  : return not_equal_vector_vector(a.as<vdouble>(), b.as<vdouble>());
        default               : ;
        }
        break;
    case Type::VecString:
        SWITCH_B
        case Type::String     : return not_equal_vector_scalar(a.as<vQString>(), b.as<QString>());
        case Type::VecString  : return not_equal_vector_vector(a.as<vQString>(), b.as<vQString>());
        default               : ;
        }
        break;
    case Type::VecDate:
        SWITCH_B
        case Type::Date       : return not_equal_vector_scalar(a.as<vQDate>(), b.as< QDate>());
        case Type::VecDate    : return not_equal_vector_vector(a.as<vQDate>(), b.as<vQDate>());
        default               : ;
        }
        break;
    case Type::VecTime:
        SWITCH_B
        case Type::Time       : return not_equal_vector_scalar(a.as<vQTime>(), b.as< QTime>());
        case Type::VecTime    : return not_equal_vector_vector(a.as<vQTime>(), b.as<vQTime>());
        default               : ;
        }
        break;
    case Type::VecDateTime:
        SWITCH_B
        case Type::DateTime   : return not_equal_vector_scalar(a.as<vQDateTime>(), b.as< QDateTime>());
        case Type::VecDateTime: return not_equal_vector_vector(a.as<vQDateTime>(), b.as<vQDateTime>());
        default               : ;
        }
        break;
    case Type::VecBareDate:
        SWITCH_B
        case Type::BareDate   : return not_equal_vector_scalar(a.as<vBareDate>(), b.as< BareDate>());
        case Type::VecBareDate: return not_equal_vector_vector(a.as<vBareDate>(), b.as<vBareDate>());
        default               : ;
        }
        break;
    }
    ThrowException("Incompatible operands in comparison").value(a.typeName() +" != "+ b.typeName());
    return Value();
}

//
// Logical and
//

Value and_(const Value &a, const Value &b) {
    using Type = Value::Type;

    SWITCH_A
    case Type::Bool:
        SWITCH_B
        case Type::Bool     : return a.as<bool>() && b.as<bool>();
        case Type::VecBool  : return and_vector_scalar(b.as<vbool>(), b.as<bool>());
        default            : ;
        }
        break;
    case Type::VecBool:
        SWITCH_B
        case Type::Bool     : return and_vector_scalar(b.as<vbool>(), a.as< bool>());
        case Type::VecBool  : return and_vector_vector(a.as<vbool>(), b.as<vbool>());
        default            : ;
        }
        break;
    default: ;
    }
    ThrowException("Incompatible operands for logical 'and'").value(a.typeName() +" && "+ b.typeName());
    return Value();
}

//
// Logical and
//

Value or_(const Value &a, const Value &b) {
    using Type = Value::Type;

    SWITCH_A
    case Type::Bool:
        SWITCH_B
        case Type::Bool     : return a.as<bool>() || b.as<bool>();
        case Type::VecBool  : return or_vector_scalar(b.as<vbool>(), b.as<bool>());
        default            : ;
        }
        break;
    case Type::VecBool:
        SWITCH_B
        case Type::Bool     : return or_vector_scalar(b.as<vbool>(), a.as< bool>());
        case Type::VecBool  : return or_vector_vector(a.as<vbool>(), b.as<vbool>());
        default            : ;
        }
        break;
    default: ;
    }
    ThrowException("Incompatible operands for logical 'or'").value(a.typeName() +" || "+ b.typeName());
    return Value();
}

//
// Logical not
//

Value not_(const Value &a) {
    using Type = Value::Type;

    SWITCH_A
    case Type::Bool:
        return Value(!a.as<bool>());
    case Type::VecBool:
        return Value(not_vector(a.as<vbool>()));
    default: ;
    }
    ThrowException("Incompatible operand for logical 'not'").value("! " + a.typeName());
    return Value();
}

//
// Negation
//

Value negate(const Value &a) {
    using Type = Value::Type;

    SWITCH_A
    case Type::Int:
        return Value(-a.as<int>());
    case Type::Double:
        return Value(-a.as<double>());
    case Type::VecInt:
        return negate_vector(a.as<vint>());
    case Type::VecDouble:
        return negate_vector(a.as<vdouble>());
    default: ;
    }
    ThrowException("Operand cannot be negated").value("- " + a.typeName());
    return Value();
}

}

