/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <boost/numeric/conversion/cast.hpp>
#include "exception.h"
#include "operation.h"

using boost::numeric_cast;
using namespace base;

namespace operation {

inline int daysToDays(double x) {
    return numeric_cast<int>(x+0.5);
}

inline int daysToSecs(double x) {
    return numeric_cast<int>(x*24*3600+0.5);
}

inline int hoursToSecs(int x) {
    return x*3600;
}

inline int hoursToSecs(double x) {
    return numeric_cast<int>(x*3600+0.5);
}

Value add(const Value &a, const Value &b) {
    switch(a.type()) {
    case Value::Type::Bool:
    case Value::Type::BoolPtr:
        break;
    case Value::Type::Char:
        switch(b.type()) {
        case Value::Type::String:       return Value{QString(get<char>(a)) + get<QString>(b)};
        default: ;
        }
        break;
    case Value::Type::CharPtr:
        switch(b.type()) {
        case Value::Type::String:       return Value{QString(*get<char*>(a)) + get<QString>(b)};
        default: ;
        }
        break;
    case Value::Type::Int:
        switch(b.type()) {
        case Value::Type::Bool:
        case Value::Type::Char:         break;
        case Value::Type::Int:          return Value{get<int>(a) + get<int>(b)};
        case Value::Type::Double:       return Value{get<int>(a) + get<double>(b)};
        case Value::Type::Date:         return Value{get<QDate    >(b).addDays(get<int>(a))};
        case Value::Type::DateTime:     return Value{get<QDateTime>(b).addDays(get<int>(a))};
        case Value::Type::Time:         return Value{get<QTime>(b).addSecs(3600*get<int>(a))};
        case Value::Type::BareDate:     return Value{get<BareDate    >(b).addBareDays(get<int>(a))};
        case Value::Type::BareDateTime: return Value{get<BareDateTime>(b).addBareDays(get<int>(a))};
        case Value::Type::String:       return Value{QString::number(get<int>(a)) + get<QString>(b)};
        case Value::Type::BoolPtr:
        case Value::Type::CharPtr:      break;
        case Value::Type::IntPtr:       return Value{get<int>(a) + *get<int*>(b)};
        case Value::Type::DoublePtr:    return Value{get<int>(a) + *get<double*>(b)};
        case Value::Type::DatePtr:      return Value{(*get<QDate*    >(b)).addDays(get<int>(a))};
        case Value::Type::DateTimePtr:  return Value{(*get<QDateTime*>(b)).addDays(get<int>(a))};
        case Value::Type::TimePtr:      return Value{(*get<QTime*>(b)).addSecs(3600*get<int>(a))};
        case Value::Type::BareDatePtr:  return Value{(*get<BareDate*    >(b)).addDays(get<int>(a))};
        case Value::Type::BareDateTimePtr: return Value{(*get<BareDateTime*>(b)).addDays(get<int>(a))};
        case Value::Type::StringPtr:    return Value{QString::number(get<int>(a)) + *get<QString*>(b)};
        }
        break;
    case Value::Type::IntPtr:
        switch(b.type()) {
        case Value::Type::Bool:
        case Value::Type::Char:         break;
        case Value::Type::Int:          return Value{*get<int*>(a) + get<int>(b)};
        case Value::Type::Double:       return Value{*get<int*>(a) + get<double>(b)};
        case Value::Type::Date:         return Value{get<QDate    >(b).addDays(*get<int*>(a))};
        case Value::Type::DateTime:     return Value{get<QDateTime>(b).addDays(*get<int*>(a))};
        case Value::Type::Time:         return Value{get<QTime>(b).addSecs(*get<int*>(a)*3600)};
        case Value::Type::BareDate:     return Value{get<BareDate    >(b).addDays(*get<int*>(a))};
        case Value::Type::BareDateTime: return Value{get<BareDateTime>(b).addDays(*get<int*>(a))};
        case Value::Type::String:       return Value{QString::number(*get<int*>(a)) + get<QString>(b)};
        case Value::Type::BoolPtr:
        case Value::Type::CharPtr:      break;
        case Value::Type::IntPtr:       return Value{*get<int*>(a) + *get<int*>(b)};
        case Value::Type::DoublePtr:    return Value{*get<int*>(a) + *get<double*>(b)};
        case Value::Type::DatePtr:      return Value{(*get<QDate*    >(b)).addDays(*get<int*>(a))};
        case Value::Type::DateTimePtr:  return Value{(*get<QDateTime*>(b)).addDays(*get<int*>(a))};
        case Value::Type::TimePtr:      return Value{(*get<QTime*>(b)).addSecs(*get<int*>(a)*3600)};
        case Value::Type::BareDatePtr:  return Value{(*get<BareDate*    >(b)).addDays(*get<int*>(a))};
        case Value::Type::BareDateTimePtr: return Value{(*get<BareDateTime*>(b)).addDays(*get<int*>(a))};
        case Value::Type::StringPtr:    return Value{QString::number(*get<int*>(a)) + *get<QString*>(b)};
        }
        break;
    case Value::Type::Double:
        switch(b.type()) {
        case Value::Type::Bool:
        case Value::Type::Char:         break;
        case Value::Type::Int:          return Value{get<double>(a) + get<int>(b)};
        case Value::Type::Double:       return Value{get<double>(a) + get<double>(b)};
        case Value::Type::Date:         return Value{get<QDate    >(b).addDays(daysToDays(get<double>(a)))};
        case Value::Type::DateTime:     return Value{get<QDateTime>(b).addSecs(daysToSecs(get<double>(a)))};
        case Value::Type::Time:         return Value{get<QTime>(b).addSecs(hoursToSecs(get<double>(a)))};
        case Value::Type::BareDate:     return Value{get<BareDate    >(b).addDays(daysToDays(get<double>(a)))};
        case Value::Type::BareDateTime: return Value{get<BareDateTime>(b).addDays(daysToDays(get<double>(a)))};
        case Value::Type::String:       return Value{QString::number(get<double>(a)) + get<QString>(b)};
        case Value::Type::BoolPtr:
        case Value::Type::CharPtr:      break;
        case Value::Type::IntPtr:       return Value{get<double>(a) + *get<int*>(b)};
        case Value::Type::DoublePtr:    return Value{get<double>(a) + *get<double*>(b)};
        case Value::Type::DatePtr:      return Value{(*get<QDate*    >(b)).addDays(daysToDays(get<double>(a)))};
        case Value::Type::DateTimePtr:  return Value{(*get<QDateTime*>(b)).addSecs(daysToSecs(get<double>(a)))};
        case Value::Type::TimePtr:      return Value{(*get<QTime*>(b)).addSecs(hoursToSecs(get<double>(a)))};
        case Value::Type::BareDatePtr:  return Value{(*get<BareDate* >(b)).addDays(daysToDays(get<double>(a)))};
        case Value::Type::BareDateTimePtr: return Value{(*get<BareDateTime*>(b)).addDays(daysToDays(get<double>(a)))};
        case Value::Type::StringPtr:    return Value{QString::number(get<double>(a)) + *get<QString*>(b)};
        }
        break;
    case Value::Type::DoublePtr:
        switch(b.type()) {
        case Value::Type::Bool:
        case Value::Type::Char:         break;
        case Value::Type::Int:          return Value{*get<double*>(a) + get<int>(b)};
        case Value::Type::Double:       return Value{*get<double*>(a) + get<double>(b)};
        case Value::Type::Date:         return Value{get<QDate    >(b).addDays(daysToDays(*get<double*>(a)))};
        case Value::Type::DateTime:     return Value{get<QDateTime>(b).addSecs(daysToSecs(*get<double*>(a)))};
        case Value::Type::Time:         return Value{get<QTime>(b).addSecs(hoursToSecs(*get<double*>(a)))};
        case Value::Type::BareDate:     return Value{get<BareDate    >(b).addDays(daysToDays(*get<double*>(a)))};
        case Value::Type::BareDateTime: return Value{get<BareDateTime>(b).addDays(daysToDays(*get<double*>(a)))};
        case Value::Type::String:       return Value{QString::number(*get<double*>(a)) + get<QString>(b)};
        case Value::Type::BoolPtr:
        case Value::Type::CharPtr:      break;
        case Value::Type::IntPtr:       return Value{*get<double*>(a) + *get<int*>(b)};
        case Value::Type::DoublePtr:    return Value{*get<double*>(a) + *get<double*>(b)};
        case Value::Type::DatePtr:      return Value{(*get<QDate*    >(b)).addDays(daysToDays(*get<double*>(a)))};
        case Value::Type::DateTimePtr:  return Value{(*get<QDateTime*>(b)).addSecs(daysToSecs(*get<double*>(a)))};
        case Value::Type::TimePtr:      return Value{(*get<QTime*>(b)).addSecs(hoursToSecs(*get<double*>(a)))};
        case Value::Type::BareDatePtr:  return Value{(*get<BareDate* >(b)).addDays(daysToDays(*get<double*>(a)))};
        case Value::Type::BareDateTimePtr: return Value{(*get<BareDateTime*>(b)).addDays(daysToDays(*get<double*>(a)))};
        case Value::Type::StringPtr:    return Value{QString::number(*get<double*>(a)) + *get<QString*>(b)};
        }
        break;
    case Value::Type::Date:
    case Value::Type::DatePtr:
    case Value::Type::DateTime:
    case Value::Type::DateTimePtr:
    case Value::Type::Time:
    case Value::Type::TimePtr:
    case Value::Type::BareDate:
    case Value::Type::BareDatePtr:
    case Value::Type::BareDateTime:
    case Value::Type::BareDateTimePtr:
        switch(b.type()) {
        case Value::Type::Int:          return add(b,a);
        case Value::Type::Double:       return add(b,a);
        case Value::Type::IntPtr:       return add(b,a);
        case Value::Type::DoublePtr:    return add(b,a);
        case Value::Type::String:       return add(b,a);
        case Value::Type::StringPtr:    return add(b,a);
        default: ;
        }
        break;
    case Value::Type::String:
    case Value::Type::StringPtr:
        return Value{get<QString>(a) + b.toString()};
    }
    ThrowException("Incompatile Values for addition").
            value(a.toString() + " + " + b.toString()).
            value2(a.typeName() + " + " + b.typeName());
}

Value subtract(const Value &a, const Value &b) {
    switch(a.type()) {
    case Value::Type::Bool:
    case Value::Type::BoolPtr:
    case Value::Type::Char:
    case Value::Type::CharPtr:
        break;
    case Value::Type::Int:
        switch(b.type()) {
        case Value::Type::Bool:
        case Value::Type::Char:         break;
        case Value::Type::Int:          return Value{get<int>(a) - get<int>(b)};
        case Value::Type::Double:       return Value{get<int>(a) - get<double>(b)};
        case Value::Type::Date:
        case Value::Type::DateTime:
        case Value::Type::Time:
        case Value::Type::BareDate:
        case Value::Type::BareDateTime:
        case Value::Type::String:
        case Value::Type::BoolPtr:
        case Value::Type::CharPtr:      break;
        case Value::Type::IntPtr:       return Value{get<int>(a) - *get<int*>(b)};
        case Value::Type::DoublePtr:    return Value{get<int>(a) - *get<double*>(b)};
        case Value::Type::DatePtr:
        case Value::Type::DateTimePtr:
        case Value::Type::TimePtr:
        case Value::Type::BareDatePtr:
        case Value::Type::BareDateTimePtr:
        case Value::Type::StringPtr:   break;
        }
        break;
    case Value::Type::IntPtr:
        switch(b.type()) {
        case Value::Type::Bool:
        case Value::Type::Char:         break;
        case Value::Type::Int:          return Value{*get<int*>(a) - get<int>(b)};
        case Value::Type::Double:       return Value{*get<int*>(a) - get<double>(b)};
        case Value::Type::Date:
        case Value::Type::DateTime:
        case Value::Type::Time:
        case Value::Type::BareDate:
        case Value::Type::BareDateTime:
        case Value::Type::String:
        case Value::Type::BoolPtr:
        case Value::Type::CharPtr:      break;
        case Value::Type::IntPtr:       return Value{*get<int*>(a) - *get<int*>(b)};
        case Value::Type::DoublePtr:    return Value{*get<int*>(a) - *get<double*>(b)};
        case Value::Type::DatePtr:
        case Value::Type::DateTimePtr:
        case Value::Type::TimePtr:
        case Value::Type::BareDatePtr:
        case Value::Type::BareDateTimePtr:
        case Value::Type::StringPtr:   break;
        }
        break;
    case Value::Type::Double:
        switch(b.type()) {
        case Value::Type::Bool:
        case Value::Type::Char:         break;
        case Value::Type::Int:          return Value{get<double>(a) - get<int>(b)};
        case Value::Type::Double:       return Value{get<double>(a) - get<double>(b)};
        case Value::Type::Date:
        case Value::Type::DateTime:
        case Value::Type::Time:
        case Value::Type::BareDate:
        case Value::Type::BareDateTime:
        case Value::Type::String:
        case Value::Type::BoolPtr:
        case Value::Type::CharPtr:      break;
        case Value::Type::IntPtr:       return Value{get<double>(a) - *get<int*>(b)};
        case Value::Type::DoublePtr:    return Value{get<double>(a) - *get<double*>(b)};
        case Value::Type::DatePtr:
        case Value::Type::DateTimePtr:
        case Value::Type::TimePtr:
        case Value::Type::BareDatePtr:
        case Value::Type::BareDateTimePtr:
        case Value::Type::StringPtr:   break;
        }
        break;
    case Value::Type::DoublePtr:
        switch(b.type()) {
        case Value::Type::Bool:
        case Value::Type::Char:         break;
        case Value::Type::Int:          return Value{*get<double*>(a) - get<int>(b)};
        case Value::Type::Double:       return Value{*get<double*>(a) - get<double>(b)};
        case Value::Type::Date:
        case Value::Type::DateTime:
        case Value::Type::Time:
        case Value::Type::BareDate:
        case Value::Type::BareDateTime:
        case Value::Type::String:
        case Value::Type::BoolPtr:
        case Value::Type::CharPtr:      break;
        case Value::Type::IntPtr:       return Value{*get<double*>(a) - *get<int*>(b)};
        case Value::Type::DoublePtr:    return Value{*get<double*>(a) - *get<double*>(b)};
        case Value::Type::DatePtr:
        case Value::Type::DateTimePtr:
        case Value::Type::TimePtr:
        case Value::Type::BareDatePtr:
        case Value::Type::BareDateTimePtr:
        case Value::Type::StringPtr:   break;
        }
        break;
    case Value::Type::Date:
        switch(b.type()) {
        case Value::Type::Int:          return Value{get<QDate>(a).addDays(-get<int>(b))};
        case Value::Type::IntPtr:       return Value{get<QDate>(a).addDays(-(*get<int*>(b)))};
        case Value::Type::Double:       return Value{get<QDate>(a).addDays(-daysToDays(get<double>(b)))};
        case Value::Type::DoublePtr:    return Value{get<QDate>(a).addDays(-daysToDays(*get<double*>(b)))};
        default: ;
        }
        break;
    case Value::Type::DatePtr:
        switch(b.type()) {
        case Value::Type::Int:          return Value{(*get<QDate*>(a)).addDays(-get<int>(b))};
        case Value::Type::IntPtr:       return Value{(*get<QDate*>(a)).addDays(-(*get<int*>(b)))};
        case Value::Type::Double:       return Value{(*get<QDate*>(a)).addDays(-daysToDays(get<double>(b)))};
        case Value::Type::DoublePtr:    return Value{(*get<QDate*>(a)).addDays(-daysToDays(*get<double*>(b)))};
        default: ;
        }
        break;
    case Value::Type::DateTime:
        switch(b.type()) {
        case Value::Type::Int:          return Value{get<QDateTime>(a).addDays(-get<int>(b))};
        case Value::Type::IntPtr:       return Value{get<QDateTime>(a).addDays(-(*get<int*>(b)))};
        case Value::Type::Double:       return Value{get<QDateTime>(a).addDays(-daysToDays(get<double>(b)))};
        case Value::Type::DoublePtr:    return Value{get<QDateTime>(a).addDays(-daysToDays(*get<double*>(b)))};
        default: ;
        }
        break;
    case Value::Type::DateTimePtr:
        switch(b.type()) {
        case Value::Type::Int:          return Value{(*get<QDateTime*>(a)).addDays(-get<int>(b))};
        case Value::Type::IntPtr:       return Value{(*get<QDateTime*>(a)).addDays(-(*get<int*>(b)))};
        case Value::Type::Double:       return Value{(*get<QDateTime*>(a)).addDays(-daysToDays(get<double>(b)))};
        case Value::Type::DoublePtr:    return Value{(*get<QDateTime*>(a)).addDays(-daysToDays(*get<double*>(b)))};
        default: ;
        }
        break;
    case Value::Type::Time:
        switch(b.type()) {
        case Value::Type::Int:          return Value{get<QTime>(a).addSecs(3600*get<int>(b))};
        case Value::Type::IntPtr:       return Value{get<QTime>(a).addSecs(3600*(*get<int*>(b)))};
        case Value::Type::Double:       return Value{get<QTime>(a).addSecs(hoursToSecs(get<double>(b)))};
        case Value::Type::DoublePtr:    return Value{get<QTime>(a).addSecs(hoursToSecs(*get<double*>(b)))};
        default: ;
        }
        break;
    case Value::Type::TimePtr:
        switch(b.type()) {
        case Value::Type::Int:          return Value{(*get<QTime*>(a)).addSecs(3600*get<int>(b))};
        case Value::Type::IntPtr:       return Value{(*get<QTime*>(a)).addSecs(3600*(*get<int*>(b)))};
        case Value::Type::Double:       return Value{(*get<QTime*>(a)).addSecs(hoursToSecs(get<double>(b)))};
        case Value::Type::DoublePtr:    return Value{(*get<QTime*>(a)).addSecs(hoursToSecs(*get<double*>(b)))};
        default: ;
        }
        break;
    case Value::Type::BareDate:
        switch(b.type()) {
        case Value::Type::Int:          return Value{get<BareDate>(a).addDays(-get<int>(b))};
        case Value::Type::IntPtr:       return Value{get<BareDate>(a).addDays(-(*get<int*>(b)))};
        case Value::Type::Double:       return Value{get<BareDate>(a).addDays(-daysToDays(get<double>(b)))};
        case Value::Type::DoublePtr:    return Value{get<BareDate>(a).addDays(-daysToDays(*get<double*>(b)))};
        default: ;
        }
        break;
    case Value::Type::BareDatePtr:
        switch(b.type()) {
        case Value::Type::Int:          return Value{(*get<BareDate*>(a)).addDays(-get<int>(b))};
        case Value::Type::IntPtr:       return Value{(*get<BareDate*>(a)).addDays(-(*get<int*>(b)))};
        case Value::Type::Double:       return Value{(*get<BareDate*>(a)).addDays(-daysToDays(get<double>(b)))};
        case Value::Type::DoublePtr:    return Value{(*get<BareDate*>(a)).addDays(-daysToDays(*get<double*>(b)))};
        default: ;
        }
        break;
    case Value::Type::BareDateTime:
        switch(b.type()) {
        case Value::Type::Int:          return Value{get<BareDateTime>(a).addDays(-get<int>(b))};
        case Value::Type::IntPtr:       return Value{get<BareDateTime>(a).addDays(-(*get<int*>(b)))};
        case Value::Type::Double:       return Value{get<BareDateTime>(a).addDays(-daysToDays(get<double>(b)))};
        case Value::Type::DoublePtr:    return Value{get<BareDateTime>(a).addDays(-daysToDays(*get<double*>(b)))};
        default: ;
        }
        break;
    case Value::Type::BareDateTimePtr:
        switch(b.type()) {
        case Value::Type::Int:          return Value{(*get<BareDateTime*>(a)).addDays(-get<int>(b))};
        case Value::Type::IntPtr:       return Value{(*get<BareDateTime*>(a)).addDays(-(*get<int*>(b)))};
        case Value::Type::Double:       return Value{(*get<BareDateTime*>(a)).addDays(-daysToDays(get<double>(b)))};
        case Value::Type::DoublePtr:    return Value{(*get<BareDateTime*>(a)).addDays(-daysToDays(*get<double*>(b)))};
        default: ;
        }
        break;
    case Value::Type::String:
    case Value::Type::StringPtr: break;
    }
    ThrowException("Incompatile Values for subtraction").
            value(a.toString() + " + " + b.toString()).
            value2(a.typeName() + " + " + b.typeName());
}

Value multiply(const Value &a, const Value &b) {
    switch(a.type()) {
    case Value::Type::Bool:
    case Value::Type::BoolPtr:
    case Value::Type::Char:
    case Value::Type::CharPtr:
        break;
    case Value::Type::Int:
        switch(b.type()) {
        case Value::Type::Bool:
        case Value::Type::Char:         break;
        case Value::Type::Int:          return Value{get<int>(a) * get<int>(b)};
        case Value::Type::Double:       return Value{get<int>(a) * get<double>(b)};
        case Value::Type::Date:
        case Value::Type::DateTime:
        case Value::Type::Time:
        case Value::Type::BareDate:
        case Value::Type::BareDateTime:
        case Value::Type::String:
        case Value::Type::BoolPtr:
        case Value::Type::CharPtr:      break;
        case Value::Type::IntPtr:       return Value{get<int>(a) * (*get<int*>(b))};
        case Value::Type::DoublePtr:    return Value{get<int>(a) * (*get<double*>(b))};
        case Value::Type::DatePtr:
        case Value::Type::DateTimePtr:
        case Value::Type::TimePtr:
        case Value::Type::BareDatePtr:
        case Value::Type::BareDateTimePtr:
        case Value::Type::StringPtr:   break;
        }
        break;
    case Value::Type::IntPtr:
        switch(b.type()) {
        case Value::Type::Bool:
        case Value::Type::Char:         break;
        case Value::Type::Int:          return Value{*get<int*>(a) * get<int>(b)};
        case Value::Type::Double:       return Value{*get<int*>(a) * get<double>(b)};
        case Value::Type::Date:
        case Value::Type::DateTime:
        case Value::Type::Time:
        case Value::Type::BareDate:
        case Value::Type::BareDateTime:
        case Value::Type::String:
        case Value::Type::BoolPtr:
        case Value::Type::CharPtr:      break;
        case Value::Type::IntPtr:       return Value{*get<int*>(a) * (*get<int*>(b))};
        case Value::Type::DoublePtr:    return Value{*get<int*>(a) * (*get<double*>(b))};
        case Value::Type::DatePtr:
        case Value::Type::DateTimePtr:
        case Value::Type::TimePtr:
        case Value::Type::BareDatePtr:
        case Value::Type::BareDateTimePtr:
        case Value::Type::StringPtr:   break;
        }
        break;
    case Value::Type::Double:
        switch(b.type()) {
        case Value::Type::Bool:
        case Value::Type::Char:         break;
        case Value::Type::Int:          return Value{get<double>(a) * get<int>(b)};
        case Value::Type::Double:       return Value{get<double>(a) * get<double>(b)};
        case Value::Type::Date:
        case Value::Type::DateTime:
        case Value::Type::Time:
        case Value::Type::BareDate:
        case Value::Type::BareDateTime:
        case Value::Type::String:
        case Value::Type::BoolPtr:
        case Value::Type::CharPtr:      break;
        case Value::Type::IntPtr:       return Value{get<double>(a) * (*get<int*>(b))};
        case Value::Type::DoublePtr:    return Value{get<double>(a) * (*get<double*>(b))};
        case Value::Type::DatePtr:
        case Value::Type::DateTimePtr:
        case Value::Type::TimePtr:
        case Value::Type::BareDatePtr:
        case Value::Type::BareDateTimePtr:
        case Value::Type::StringPtr:   break;
        }
        break;
    case Value::Type::DoublePtr:
        switch(b.type()) {
        case Value::Type::Bool:
        case Value::Type::Char:         break;
        case Value::Type::Int:          return Value{*get<double*>(a) * get<int>(b)};
        case Value::Type::Double:       return Value{*get<double*>(a) * get<double>(b)};
        case Value::Type::Date:
        case Value::Type::DateTime:
        case Value::Type::Time:
        case Value::Type::BareDate:
        case Value::Type::BareDateTime:
        case Value::Type::String:
        case Value::Type::BoolPtr:
        case Value::Type::CharPtr:      break;
        case Value::Type::IntPtr:       return Value{*get<double*>(a) * (*get<int*>(b))};
        case Value::Type::DoublePtr:    return Value{*get<double*>(a) * (*get<double*>(b))};
        case Value::Type::DatePtr:
        case Value::Type::DateTimePtr:
        case Value::Type::TimePtr:
        case Value::Type::BareDatePtr:
        case Value::Type::BareDateTimePtr:
        case Value::Type::StringPtr:   break;
        }
        break;
    case Value::Type::Date:
    case Value::Type::DatePtr:
    case Value::Type::DateTime:
    case Value::Type::DateTimePtr:
    case Value::Type::Time:
    case Value::Type::TimePtr:
    case Value::Type::BareDate:
    case Value::Type::BareDatePtr:
    case Value::Type::BareDateTime:
    case Value::Type::BareDateTimePtr:
    case Value::Type::String:
    case Value::Type::StringPtr: break;
    }
    ThrowException("Incompatile Values for multiplication").
            value(a.toString() + " + " + b.toString()).
            value2(a.typeName() + " + " + b.typeName());
}

Value divide(const Value &a, const Value &b) {
    switch(a.type()) {
    case Value::Type::Bool:
    case Value::Type::BoolPtr:
    case Value::Type::Char:
    case Value::Type::CharPtr:
        break;
    case Value::Type::Int:
        switch(b.type()) {
        case Value::Type::Bool:
        case Value::Type::Char:         break;
        case Value::Type::Int:          return Value{get<int>(a) / numeric_cast<double>(get<int>(b))};
        case Value::Type::Double:       return Value{get<int>(a) / get<double>(b)};
        case Value::Type::Date:
        case Value::Type::DateTime:
        case Value::Type::Time:
        case Value::Type::BareDate:
        case Value::Type::BareDateTime:
        case Value::Type::String:
        case Value::Type::BoolPtr:
        case Value::Type::CharPtr:      break;
        case Value::Type::IntPtr:       return Value{get<int>(a) / numeric_cast<double>(*get<int*>(b))};
        case Value::Type::DoublePtr:    return Value{get<int>(a) / (*get<double*>(b))};
        case Value::Type::DatePtr:
        case Value::Type::DateTimePtr:
        case Value::Type::TimePtr:
        case Value::Type::BareDatePtr:
        case Value::Type::BareDateTimePtr:
        case Value::Type::StringPtr:   break;
        }
        break;
    case Value::Type::IntPtr:
        switch(b.type()) {
        case Value::Type::Bool:
        case Value::Type::Char:         break;
        case Value::Type::Int:          return Value{*get<int*>(a) / numeric_cast<double>(*get<int*>(b))};
        case Value::Type::Double:       return Value{*get<int*>(a) / get<double>(b)};
        case Value::Type::Date:
        case Value::Type::DateTime:
        case Value::Type::Time:
        case Value::Type::BareDate:
        case Value::Type::BareDateTime:
        case Value::Type::String:
        case Value::Type::BoolPtr:
        case Value::Type::CharPtr:      break;
        case Value::Type::IntPtr:       return Value{*get<int*>(a) / numeric_cast<double>(*get<int*>(b))};
        case Value::Type::DoublePtr:    return Value{*get<int*>(a) / (*get<double*>(b))};
        case Value::Type::DatePtr:
        case Value::Type::DateTimePtr:
        case Value::Type::TimePtr:
        case Value::Type::BareDatePtr:
        case Value::Type::BareDateTimePtr:
        case Value::Type::StringPtr:   break;
        }
        break;
    case Value::Type::Double:
        switch(b.type()) {
        case Value::Type::Bool:
        case Value::Type::Char:         break;
        case Value::Type::Int:          return Value{get<double>(a) / get<int>(b)};
        case Value::Type::Double:       return Value{get<double>(a) / get<double>(b)};
        case Value::Type::Date:
        case Value::Type::DateTime:
        case Value::Type::Time:
        case Value::Type::BareDate:
        case Value::Type::BareDateTime:
        case Value::Type::String:
        case Value::Type::BoolPtr:
        case Value::Type::CharPtr:      break;
        case Value::Type::IntPtr:       return Value{get<double>(a) / (*get<int*>(b))};
        case Value::Type::DoublePtr:    return Value{get<double>(a) / (*get<double*>(b))};
        case Value::Type::DatePtr:
        case Value::Type::DateTimePtr:
        case Value::Type::TimePtr:
        case Value::Type::BareDatePtr:
        case Value::Type::BareDateTimePtr:
        case Value::Type::StringPtr:   break;
        }
        break;
    case Value::Type::DoublePtr:
        switch(b.type()) {
        case Value::Type::Bool:
        case Value::Type::Char:         break;
        case Value::Type::Int:          return Value{*get<double*>(a) / get<int>(b)};
        case Value::Type::Double:       return Value{*get<double*>(a) / get<double>(b)};
        case Value::Type::Date:
        case Value::Type::DateTime:
        case Value::Type::Time:
        case Value::Type::BareDate:
        case Value::Type::BareDateTime:
        case Value::Type::String:
        case Value::Type::BoolPtr:
        case Value::Type::CharPtr:      break;
        case Value::Type::IntPtr:       return Value{*get<double*>(a) / (*get<int*>(b))};
        case Value::Type::DoublePtr:    return Value{*get<double*>(a) / (*get<double*>(b))};
        case Value::Type::DatePtr:
        case Value::Type::DateTimePtr:
        case Value::Type::TimePtr:
        case Value::Type::BareDatePtr:
        case Value::Type::BareDateTimePtr:
        case Value::Type::StringPtr:   break;
        }
        break;
    case Value::Type::Date:
    case Value::Type::DatePtr:
    case Value::Type::DateTime:
    case Value::Type::DateTimePtr:
    case Value::Type::Time:
    case Value::Type::TimePtr:
    case Value::Type::BareDate:
    case Value::Type::BareDatePtr:
    case Value::Type::BareDateTime:
    case Value::Type::BareDateTimePtr:
    case Value::Type::String:
    case Value::Type::StringPtr: break;
    }
    ThrowException("Incompatile Values for division").
            value(a.toString() + " + " + b.toString()).
            value2(a.typeName() + " + " + b.typeName());
}

}
