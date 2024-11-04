/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_CONVERT_H
#define BASE_CONVERT_H

#include <typeinfo>
#include <boost/numeric/conversion/cast.hpp>
#include <QDate>
#include <QDateTime>
#include <QString>
#include <QTime>
#include <QVector>
#include "bare_date.h"
#include "exception.h"
#include "path.h"

namespace base {

class Value;

using vbool      = QVector<bool>;
using vint       = QVector<int>;
using vdouble    = QVector<double>;
using vQString   = QVector<QString>;
using vQDate     = QVector<QDate>;
using vQTime     = QVector<QTime>;
using vQDateTime = QVector<QDateTime>;
using vBareDate  = QVector<BareDate>;
using vValuePtr  = QVector<const Value*>;

template <class, template <class> class>
struct isInstance : public std::false_type {};

template <class T, template <class> class U>
struct isInstance<U<T>, U> : public std::true_type {};

// Define conversions to type T from type U
template <class T, class U> T convert(U ) {
    ThrowException("Type conversion not supported")
            .value1(QString("From: ") + typeid(U).name())
            .value2(QString("To  : ") + typeid(T).name());
}

// Check if T is convertible to U; if so result is returned in y
template <class T, class U> bool canConvert(U x) {
    try {
        convert<T>(x);
    }
    catch(const Exception &) {
        return false;
    }
    return true;
}

// Needed forward declarations
template<> inline QString convert(QDate x);
template<> inline QString convert(QTime x);
template<> inline QString convert(QDateTime x);
template<> inline QString convert(BareDate x);

// Conversions to bool
template<> inline bool convert(bool x)   {return x;}
template<> inline bool convert(int x)    {return x;}
template<> inline bool convert(double x) {return x;}
template<> inline bool convert(QString x){ if      (x=="TRUE"  || x=="1") return true;
                                           else if (x=="FALSE" || x=="0") return false;
                                           ThrowException("Cannot throw string to bool")
                                           .value(x);}
template<> inline bool convert(QDate x)  {ThrowException("Cannot convert date to bool")
                                          .value(convert<QString>(x));
                                          return false;}
template<> inline bool convert(QTime x)  {ThrowException("Cannot convert time to bool")
                                          .value(convert<QString>(x));
                                          return false;}
template<> inline bool convert(QDateTime x) {ThrowException("Cannot convert date-time to bool")
                                          .value(convert<QString>(x));
                                          return false;}
template<> inline bool convert(BareDate x)  {ThrowException("Cannot convert bare date to bool")
                                          .value(convert<QString>(x));
                                          return false;}
template<> inline bool convert(const char *x) {return convert<bool>(QString(x));}

// Conversions to int
template<> inline int convert(bool x)    {return x;}
template<> inline int convert(int x)     {return x;}
template<> inline int convert(double x)  {return boost::numeric_cast<int>(x+0.5);}
template<> inline int convert(QString x) {bool ok;
                                          int value = x.toInt(&ok);
                                          if (!ok) ThrowException("Cannot convert string to int").value(x);
                                          return value;
                                         }
template<> inline int convert(QDate x)   {return x.dayOfYear();}
template<> inline int convert(QTime x)   {return x.hour();}
template<> inline int convert(QDateTime x) {return x.date().dayOfYear();}
template<> inline int convert(BareDate x)  {return x.date().dayOfYear();}
template<> inline int convert(const char *x)  {return convert<int>(QString(x));}

// Conversions to double
template<> inline double convert(bool x)   {return x;}
template<> inline double convert(int x)    {return x;}
template<> inline double convert(double x) {return x;}
template<> inline double convert(QString x){bool ok;
                                            double value = x.toDouble(&ok);
                                            if (!ok) ThrowException("Cannot convert string to double").value(x);
                                            return value;
                                           }
template<> inline double convert(QDate x)  {return x.dayOfYear();}
template<> inline double convert(QTime x)  {return x.hour() + x.minute()/60. + x.second()/3600.;}
template<> inline double convert(QDateTime x)   {return convert<double>(x.date()) +
                                                        convert<double>(x.time())/24.;}
template<> inline double convert(BareDate x)    {return x.date().dayOfYear();}
template<> inline double convert(const char *x) {return convert<double>(QString(x));}

// Conversions to QString
template<> inline QString convert(bool x)   {return QString::number(x);}
template<> inline QString convert(int x)    {return QString::number(x);}
template<> inline QString convert(double x) {QString s = QString::number(x);
                                             if (!(s.contains(".") || s.contains("inf") || s.contains("e")))
                                                 s += ".0";
                                             return s;
                                            }
template<> inline QString convert(QString x){return x;}
template<> inline QString convert(QDate x)  {return x.isValid() ? x.toString("yyyy/MM/dd") : "null";}
template<> inline QString convert(QTime x)  {return x.isValid() ? x.toString("hh:mm:ss")   : "null";}
template<> inline QString convert(QDateTime x)    {return x.date().isValid() ?
                                                   convert<QString>(x.date()) +
                                                   "T" +  convert<QString>(x.time()) : "null";}
template<> inline QString convert(BareDate x)     {return x.date().isValid() ?
                                                    convert<QString>(x.date().day()) +
                                                    "/" +  convert<QString>(x.date().month()) : "null";}
template<> inline QString convert(const Path &x) { return x.toString(); }
template<> inline QString convert(const char *x)  {return QString(x);}

// Conversions to QDate
template<> inline QDate convert(bool  )       {ThrowException("Cannot convert bool to date");
                                               return QDate();}
template<> inline QDate convert(int  )        {ThrowException("Cannot convert int to date");
                                               return QDate();}
template<> inline QDate convert(double  )     {ThrowException("Cannot convert double to date");
                                               return QDate();}
template<>        QDate convert(QString x);
template<> inline QDate convert(QDate x)      {return x;}
template<> inline QDate convert(QTime x)      {ThrowException("Cannot convert time to date").
                                               value(convert<QString>(x));
                                               return QDate();}
template<> inline QDate convert(QDateTime x)  {return x.date();}
template<> inline QDate convert(BareDate x)   {ThrowException("Cannot convert bare date to date").
                                               value(convert<QString>(x));
                                               return QDate();}
template<> inline QDate convert(const char *x)  {return convert<QDate>(QString(x));}

// Conversions to QTime
QPair<QTime, int> convertGuarded(QString s);
QPair<QTime, int> convertGuarded(int h, int m, int s);

template<> inline QTime convert(bool  )       {ThrowException("Cannot convert bool to time");
                                               return QTime();}
template<> inline QTime convert(int  )        {ThrowException("Cannot convert int to time");
                                               return QTime();}
template<> inline QTime convert(double  )     {ThrowException("Cannot convert double to time");
                                               return QTime();}
template<>        QTime convert(QString x);
template<> inline QTime convert(QDate x)      {ThrowException("Cannot convert date to time").
                                               value(convert<QString>(x));
                                               return QTime();}
template<> inline QTime convert(QTime x)      {return x;}
template<> inline QTime convert(QDateTime x)  {return x.time();}
template<> inline QTime convert(BareDate x)   {ThrowException("Cannot convert bare date to time").
                                               value(convert<QString>(x));
                                               return QTime();}
template<> inline QTime convert(const char *x)  {return convert<QTime>(QString(x));}

// Conversions to QDateTime
template<> inline QDateTime convert(bool  )     {ThrowException("Cannot convert bool to date-time");
                                                 return QDateTime();}
template<> inline QDateTime convert(int  )      {ThrowException("Cannot convert int to date-time");
                                                 return QDateTime();}
template<> inline QDateTime convert(double  )   {ThrowException("Cannot convert double to date-time");
                                                 return QDateTime();}
template<>        QDateTime convert(QString x);
template<> inline QDateTime convert(QDate x)    {return QDateTime(x, QTime(0,0), Qt::UTC);}
template<> inline QDateTime convert(QTime x)    {ThrowException("Cannot convert time to date-time").
                                                 value(convert<QString>(x));
                                                 return QDateTime();}
template<> inline QDateTime convert(QDateTime x){return x;}
template<> inline QDateTime convert(BareDate x) {ThrowException("Cannot convert bare date to date-time").
                                                 value(convert<QString>(x));
                                                 return QDateTime();}
template<> inline QDateTime convert(const char *x)  {return convert<QDateTime>(QString(x));}

// Conversions to BareDate
template<> inline BareDate convert(bool  )         {ThrowException("Cannot convert bool to bare date");
                                                    return BareDate();}
template<> inline BareDate convert(int  )          {ThrowException("Cannot convert int to bare date");
                                                    return BareDate();}
template<> inline BareDate convert(double  )       {ThrowException("Cannot convert double to bare date");
                                                    return BareDate();}
template<>        BareDate convert(QString x);
template<> inline BareDate convert(QDate x)        {return BareDate(x.month(), x.day());}
template<> inline BareDate convert(QTime x)        {ThrowException("Cannot convert time to bare date").
                                                    value(convert<QString>(x));
                                                    return BareDate();}
template<> inline BareDate convert(QDateTime x)    {ThrowException("Cannot convert date-time to bare date").
                                                    value(convert<QString>(x));
                                                    return BareDate();}
template<> inline BareDate convert(BareDate x)     {return x;}
template<> inline BareDate convert(const char *x)  {return convert<BareDate>(QString(x));}

// Conversions to Path
template<> inline Path convert(QString x)          {return Path(x); }
template<> inline Path convert(const Path &x)      {return x; }
template<> inline Path convert(Path x)             {return x; }

// Vectors are converted element-by-element
template <class T, class U> QVector<T> convertVec(U x) {
    auto source = x.data();
    int n = x.size();
    QVector<T> result(n);
    auto dest = result.data();
    for (int i=0; i<n; ++i)
        *dest++ = convert<T>(*source++);
    return result;
}

// Identical vectors need no conversion
template <class T> inline QVector<T> convertVec(QVector<T> x) {
    return x;
}

#define CONVERT_VECTOR(TYPE, VTYPE) \
template<> inline VTYPE convert(vbool x)      {return convertVec<TYPE>(x);} \
template<> inline VTYPE convert(vint x)       {return convertVec<TYPE>(x);} \
template<> inline VTYPE convert(vdouble x)    {return convertVec<TYPE>(x);} \
template<> inline VTYPE convert(vQString x)   {return convertVec<TYPE>(x);} \
template<> inline VTYPE convert(vQDate x)     {return convertVec<TYPE>(x);} \
template<> inline VTYPE convert(vQTime x)     {return convertVec<TYPE>(x);} \
template<> inline VTYPE convert(vQDateTime x) {return convertVec<TYPE>(x);} \
template<> inline VTYPE convert(vBareDate x)  {return convertVec<TYPE>(x);}

// Conversions from vector to vector
CONVERT_VECTOR(bool     , vbool)
CONVERT_VECTOR(int      , vint)
CONVERT_VECTOR(double   , vdouble)
CONVERT_VECTOR(QString  , vQString)
CONVERT_VECTOR(QDate    , vQDate)
CONVERT_VECTOR(QTime    , vQTime)
CONVERT_VECTOR(QDateTime, vQDateTime)
CONVERT_VECTOR(BareDate , vBareDate)

// Conversions to vector from values
template<> vbool      convert(vValuePtr values);
template<> vint       convert(vValuePtr values);
template<> vdouble    convert(vValuePtr values);
template<> vQString   convert(vValuePtr values);
template<> vQDate     convert(vValuePtr values);
template<> vQTime     convert(vValuePtr values);
template<> vQDateTime convert(vValuePtr values);
template<> vBareDate  convert(vValuePtr values);

// A scalar is converted to a vector with one element
template <class T, class U> QVector<T> convertVecScalar(U x) {
    return QVector<T>(1, convert<T>(x));
}

#define CONVERT_VECTOR_SCALAR(TYPE, VTYPE) \
template<> inline VTYPE convert(bool x)      {return convertVecScalar<TYPE>(x);} \
template<> inline VTYPE convert(int x)       {return convertVecScalar<TYPE>(x);} \
template<> inline VTYPE convert(double x)    {return convertVecScalar<TYPE>(x);} \
template<> inline VTYPE convert(QString x)   {return convertVecScalar<TYPE>(x);} \
template<> inline VTYPE convert(QDate x)     {return convertVecScalar<TYPE>(x);} \
template<> inline VTYPE convert(QTime x)     {return convertVecScalar<TYPE>(x);} \
template<> inline VTYPE convert(QDateTime x) {return convertVecScalar<TYPE>(x);} \
template<> inline VTYPE convert(BareDate x)  {return convertVecScalar<TYPE>(x);}

// Conversions from scalar to vector
CONVERT_VECTOR_SCALAR(bool     , vbool)
CONVERT_VECTOR_SCALAR(int      , vint)
CONVERT_VECTOR_SCALAR(double   , vdouble)
CONVERT_VECTOR_SCALAR(QString  , vQString)
CONVERT_VECTOR_SCALAR(QDate    , vQDate)
CONVERT_VECTOR_SCALAR(QTime    , vQTime)
CONVERT_VECTOR_SCALAR(QDateTime, vQDateTime)
CONVERT_VECTOR_SCALAR(BareDate , vBareDate)


// Vector conversions from QStringList to container
template<class T, template<class> class C> C<T> convert(QStringList list) {
    C<T> result;
    result.reserve(list.size());
    for (QString s : list.toVector())
        result << convert<T>(s);
    return result;
}

}
#endif
