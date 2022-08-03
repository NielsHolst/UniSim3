/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QStringList>
#include "convert.h"
#include "port.h"
#include "value.h"

namespace base {

inline QString str(bool stringent, QString s) {
    return stringent ? s : "\"" + s + "\"";
}

inline QString str(bool stringent, bool x) {
    return stringent ? convert<QString>(x) : x ? "TRUE" : "FALSE";
}

QString Value::asString() const {
    return asString(false);
}

QString Value::asStringentString() const {
    return asString(true);
}

QString Value::asString(bool stringent) const {
    QStringList slist;
    switch(type()) {
    case Type::Uninitialized: slist += "uninitialized"; break;
    case Type::Null         : slist += "null"; break;
    case Type::Bool         : slist += str(stringent, as<bool>()); break;
    case Type::VecBool      : for (auto x : as<vbool     >())
                                slist += str(stringent, x);
                              break;
    case Type::VecInt       : for (auto x : as<vint      >()) slist += convert<QString>(x);; break;
    case Type::VecDouble    : for (auto x : as<vdouble   >()) slist += convert<QString>(x);; break;
    case Type::VecDate      : for (auto x : as<vQDate    >()) slist += convert<QString>(x);; break;
    case Type::VecTime      : for (auto x : as<vQTime    >()) slist += convert<QString>(x);; break;
    case Type::VecDateTime  : for (auto x : as<vQDateTime>()) slist += convert<QString>(x);; break;
    case Type::VecBareDate  : for (auto x : as<vBareDate >()) slist += convert<QString>(x);; break;

    case Type::String       : slist += str(stringent, as<QString>());
                              break;
    case Type::VecString    : for (QString x : as<vQString  >())
                                slist += str(stringent, x);
                              break;
    case Type::Path         : slist += as<Path>().toString();
                              break;
    default                 : slist += as<QString>();
                              break;
    }
    if (isVector()) {
       return "c(" + slist.join(",") +")";
    }
    else {
       return slist.at(0);
    }
}

void Value::clear() {
    switch(type()) {
    case Type::Uninitialized: ThrowException("Cannot clear uninitialized value"); break;
    case Type::Null         : ThrowException("Cannot clear null value"); break;
    case Type::Bool         : changeValue(bool     ()); break;
    case Type::Int          : changeValue(int      ()); break;
    case Type::Double       : changeValue(double   ()); break;
    case Type::String       : changeValue(QString  ()); break;
    case Type::Date         : changeValue(QDate    ()); break;
    case Type::Time         : changeValue(QTime    ()); break;
    case Type::DateTime     : changeValue(QDateTime()); break;
    case Type::BareDate     : changeValue(BareDate ()); break;
    case Type::Path         : changeValue(Path     ()); break;
    case Type::VecBool      : changeValue(vbool      (size())); break;
    case Type::VecInt       : changeValue(vint       (size())); break;
    case Type::VecDouble    : changeValue(vdouble    (size())); break;
    case Type::VecString    : changeValue(vQString   (size())); break;
    case Type::VecDate      : changeValue(vQDate     (size())); break;
    case Type::VecTime      : changeValue(vQTime     (size())); break;
    case Type::VecDateTime  : changeValue(vQDateTime (size())); break;
    case Type::VecBareDate  : changeValue(vBareDate  (size())); break;
    }
}

int Value::size() const {
    switch(type()) {
    case Type::Uninitialized: ThrowException("Cannot tell size of uninitialized value"); break;
    case Type::Null         : return 0;                       break;
    case Type::VecBool      : return as<vbool     >().size(); break;
    case Type::VecInt       : return as<vint      >().size(); break;
    case Type::VecDouble    : return as<vdouble   >().size(); break;
    case Type::VecString    : return as<vQString  >().size(); break;
    case Type::VecDate      : return as<vQDate    >().size(); break;
    case Type::VecTime      : return as<vQTime    >().size(); break;
    case Type::VecDateTime  : return as<vQDateTime>().size(); break;
    case Type::VecBareDate  : return as<vBareDate >().size(); break;
    default: return 1;
    }
}

QString Value::outputFormat() const {
    switch(type()) {
    case Type::Date:
    case Type::VecDate: return "ymd"; break;
    case Type::Time:
    case Type::VecTime: return "HMS"; break;
    case Type::DateTime:
    case Type::VecDateTime: return "ymdHMS"; break;
    default: ;
    }
    return "NA";
}


void Value::assign(const Value &x) {
    switch(type()) {
    case Type::Uninitialized:
    case Type::Null         : _variant = x._variant         ; break;
    case Type::Bool         : changeValue(x.as<bool     >()); break;
    case Type::Int          : changeValue(x.as<int      >()); break;
    case Type::Double       : changeValue(x.as<double   >()); break;
    case Type::String       : changeValue(x.as<QString  >()); break;
    case Type::Date         : changeValue(x.as<QDate    >()); break;
    case Type::Time         : changeValue(x.as<QTime    >()); break;
    case Type::DateTime     : changeValue(x.as<QDateTime>()); break;
    case Type::BareDate     : changeValue(x.as<BareDate >()); break;
    case Type::Path         : changeValue(x.as<Path     >()); break;
    case Type::VecBool      : changeValue(x.as<QVector<bool     >>()); break;
    case Type::VecInt       : changeValue(x.as<QVector<int      >>()); break;
    case Type::VecDouble    : changeValue(x.as<QVector<double   >>()); break;
    case Type::VecString    : changeValue(x.as<QVector<QString  >>()); break;
    case Type::VecDate      : changeValue(x.as<QVector<QDate    >>()); break;
    case Type::VecTime      : changeValue(x.as<QVector<QTime    >>()); break;
    case Type::VecDateTime  : changeValue(x.as<QVector<QDateTime>>()); break;
    case Type::VecBareDate  : changeValue(x.as<QVector<BareDate >>()); break;
    }
}

bool Value::operator==(const Value &x) const {
    if (type() == x.type())
        switch(type()) {
        case Type::Uninitialized:
        case Type::Null         : return false; //true;
        case Type::Bool         : return as<bool      >() == x.as<bool      >();
        case Type::Int          : return as<int       >() == x.as<int       >();
        case Type::Double       : return as<double    >() == x.as<double    >();
        case Type::String       : return as<QString   >() == x.as<QString   >();
        case Type::Date         : return as<QDate     >() == x.as<QDate     >();
        case Type::Time         : return as<QTime     >() == x.as<QTime     >();
        case Type::DateTime     : return as<QDateTime >() == x.as<QDateTime >();
        case Type::BareDate     : return as<BareDate  >() == x.as<BareDate  >();
        case Type::Path         : return as<Path      >().toString() == x.as<Path>().toString();
        case Type::VecBool      : return as<vbool     >() == x.as<vbool     >();
        case Type::VecInt       : return as<vint      >() == x.as<vint      >();
        case Type::VecDouble    : return as<vdouble   >() == x.as<vdouble   >();
        case Type::VecString    : return as<vQString  >() == x.as<vQString  >();
        case Type::VecDate      : return as<vQDate    >() == x.as<vQDate    >();
        case Type::VecTime      : return as<vQTime    >() == x.as<vQTime    >();
        case Type::VecDateTime  : return as<vQDateTime>() == x.as<vQDateTime>();
        case Type::VecBareDate  : return as<vQDate    >() == x.as<vQDate    >();
        }
    return false;
}

#define VALUE_PTR(cppType, valueType) \
template <> const cppType* Value::constPtr() const { \
    if (type() != valueType) \
        ThrowException("Value is not of type " + QString(#cppType)).value(typeName()); \
    return std::get<ValueTyped<cppType>>(_variant).constPtr(); \
}

VALUE_PTR(bool     , Type::Bool)
VALUE_PTR(int      , Type::Int)
VALUE_PTR(double   , Type::Double)
VALUE_PTR(QString  , Type::String)
VALUE_PTR(QDate    , Type::Date)
VALUE_PTR(QTime    , Type::Time)
VALUE_PTR(QDateTime, Type::DateTime)
VALUE_PTR(BareDate , Type::BareDate)
VALUE_PTR(QVector<bool>     , Type::VecBool)
VALUE_PTR(QVector<int>      , Type::VecInt)
VALUE_PTR(QVector<double>   , Type::VecDouble)
VALUE_PTR(QVector<QString>  , Type::VecString)
VALUE_PTR(QVector<QDate>    , Type::VecDate)
VALUE_PTR(QVector<QTime>    , Type::VecTime)
VALUE_PTR(QVector<QDateTime>, Type::VecDateTime)
VALUE_PTR(QVector<BareDate> , Type::VecBareDate)

QString Value::typeName() const {
    return typeName(type());
}

QString Value::typeName(Type type){
    switch(type) {
    case Type::Uninitialized: return "uninitialized";
    case Type::Null         : return "null";
    case Type::Bool         : return "bool";
    case Type::Int          : return "int";
    case Type::Double       : return "double";
    case Type::String       : return "string";
    case Type::Date         : return "date";
    case Type::Time         : return "time";
    case Type::DateTime     : return "datetime";
    case Type::BareDate     : return "baredate";
    case Type::Path         : return "path";
    case Type::VecBool      : return "vec_bool";
    case Type::VecInt       : return "vec_int";
    case Type::VecDouble    : return "vec_double";
    case Type::VecString    : return "vec_string";
    case Type::VecDate      : return "vec_date";
    case Type::VecTime      : return "vec_time";
    case Type::VecDateTime  : return "vec_datetime";
    case Type::VecBareDate  : return "vec_baredate";
    }
    return QString();
}

Value Value::create(QString type) {
    static QMap<QString, Value> map = {
        {"", Value()},
        {"bool", Value(bool())},
        {"int", Value(int())},
        {"double", Value(double())},
        {"string", Value(QString())},
        {"date", Value(QDate())},
        {"datetime", Value(QDateTime())},
        {"baredate", Value(BareDate())},
        {"path", Value(Path())},
        {"vec_bool", Value(QVector<bool>())},
        {"vec_int", Value(QVector<int>())},
        {"vec_double", Value(QVector<double>())},
        {"vec_string", Value(QVector<QString>())},
        {"vec_date", Value(QVector<QDate>())},
        {"vec_datetime", Value(QVector<QDateTime>())},
        {"vec_baredate", Value(QVector<BareDate>())}
    };
    if (map.contains(type))
        return map.value(type);
    else
        ThrowException("Unknown value type").value(type);
}

bool Value::isNull() const {
    return type() == Type::Null;
}

}
