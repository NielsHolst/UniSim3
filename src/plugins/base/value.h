/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_VALUE_H
#define BASE_VALUE_H

#include <variant>
#include <QDate>
#include <QDateTime>
#include <QTime>
#include "bare_date.h"
#include "exception.h"
#include "path.h"
#include "value_typed.h"

namespace base {

class Port;

class Value {
public:

    struct Null {
    };
    static Value null() { return Null(); }

    enum class Type { // Add same as vectors + a QVector<Value*> for heterogenous types
        Uninitialized,
        Null,
        Bool,
        Int,
        Double,
        String,
        Date,
        Time,
        DateTime,
        BareDate,
        Path,
        VecBool,
        VecInt,
        VecDouble,
        VecString,
        VecDate,
        VecTime,
        VecDateTime,
        VecBareDate
    };

    Value() {}
    Value(Null         ) { _variant = Null(); }
    Value(bool        x) { initialize(x); }
    Value(int         x) { initialize(x); }
    Value(double      x) { initialize(x); }
    Value(QString     x) { initialize(x); }
    Value(QDate       x) { initialize(x); }
    Value(QTime       x) { initialize(x); }
    Value(QDateTime   x) { initialize(x); }
    Value(BareDate    x) { initialize(x); }
    Value(const char *x) { initialize(QString(x)); }
    explicit
    Value(Path        x) { initialize(x); }

    Value(bool       *x) { initialize(x); }
    Value(int        *x) { initialize(x); }
    Value(double     *x) { initialize(x); }
    Value(QString    *x) { initialize(x); }
    Value(QDate      *x) { initialize(x); }
    Value(QTime      *x) { initialize(x); }
    Value(QDateTime  *x) { initialize(x); }
    Value(BareDate   *x) { initialize(x); }
    explicit
    Value(Path       *x) { initialize(x); }

    Value(QVector<bool>       x) { initialize(x); }
    Value(QVector<int>        x) { initialize(x); }
    Value(QVector<double>     x) { initialize(x); }
    Value(QVector<QString>    x) { initialize(x); }
    Value(QVector<QDate>      x) { initialize(x); }
    Value(QVector<QTime>      x) { initialize(x); }
    Value(QVector<QDateTime>  x) { initialize(x); }
    Value(QVector<BareDate>   x) { initialize(x); }

    Value(QVector<bool>      *x) { initialize(x); }
    Value(QVector<int>       *x) { initialize(x); }
    Value(QVector<double>    *x) { initialize(x); }
    Value(QVector<QString>   *x) { initialize(x); }
    Value(QVector<QDate>     *x) { initialize(x); }
    Value(QVector<QTime>     *x) { initialize(x); }
    Value(QVector<QDateTime> *x) { initialize(x); }
    Value(QVector<BareDate>  *x) { initialize(x); }

    template <class T> void initialize(T *variable)
    // Link to outside variable
    {
        _variant = ValueTyped<T>(variable);
    }

    template <class T> void initialize(T value)
    // Link to inside variable
    {
        _variant = ValueTyped<T>(value);
    }

    template <class U> void changeValue(U value);
    // Set to value with compatible type U

    template <class U> void changeValueAt(U value, int i);
    // Set to value with compatible type U at vector index;
    // for scalers i must be zero

    template <class T> const T* constPtr() const;
    // Return pointer in native type T

    template <class U> U as() const;
    // Get value in compatible type U

    QString asString() const;
    // Get value converted to string for easy reading

    QString asStringentString() const;
    // Get value converted to string for stringent output

    void clear();
    // Reset value to default according to its type

    Type type() const
    // Return the value's type
    {
        return static_cast<Type>(_variant.index());
    }

    Type baseType() const
    // Return the value's type, disregarding if it's a vector
    {
        int i = _variant.index();
        if (isVector())
            i = i - static_cast<int>(Type::VecBool) + static_cast<int>(Type::Bool);
        return static_cast<Type>(i);
    }

    bool isVector() const
    // Return if this is a vectored type
    {
        return (type() >= Type::VecBool && type() <= Type::VecBareDate);
    }

    int size() const;
    // Size of vector, or 1 if scalar

    QString typeName() const;
    static QString typeName(Type type);
    // Return the value's type name

    QString outputFormat() const;
    // Format code for value in ouput file

    Value(const Value &x)            { assign(x); }
    Value& operator=(const Value &x) { assign(x);  return *this; }
    // Copy from another Value but keep my own type

    void overwrite(const Value &x) { _variant = x._variant; }
    // Overwrite value and its type

    static Value create(QString type);
    // Create a value from type name

    bool isNull() const;
    bool operator==(const Value &x) const;
    bool operator!=(const Value &x) const  { return !(*this==x); }
    // Compare
private:
    std::variant<
        std::monostate,
        Null,
        ValueTyped<bool>,
        ValueTyped<int>,
        ValueTyped<double>,
        ValueTyped<QString>,
        ValueTyped<QDate>,
        ValueTyped<QTime>,
        ValueTyped<QDateTime>,
        ValueTyped<BareDate>,
        ValueTyped<Path>,
        ValueTyped<QVector<bool>>,
        ValueTyped<QVector<int>>,
        ValueTyped<QVector<double>>,
        ValueTyped<QVector<QString>>,
        ValueTyped<QVector<QDate>>,
        ValueTyped<QVector<QTime>>,
        ValueTyped<QVector<QDateTime>>,
        ValueTyped<QVector<BareDate>>
    >
    _variant; // Starts out uninitialised (= monostate)
    void assign(const Value &x);
    QString asString(bool stringent) const;
};

template <class U> void Value::changeValue(U value)
{
    switch(type()) {
    case Type::Uninitialized:
        ThrowException("Value is uninitialized");
        break;
    case Type::Null:
        ThrowException("Value is null");
        break;
    case Type::Bool:
        std::get<ValueTyped<bool>>(_variant).changeValue(value);
        break;
    case Type::Int:
        std::get<ValueTyped<int>>(_variant).changeValue(value);
        break;
    case Type::Double:
        std::get<ValueTyped<double>>(_variant).changeValue(value);
        break;
    case Type::String:
        std::get<ValueTyped<QString>>(_variant).changeValue(value);
        break;
    case Type::Date:
        std::get<ValueTyped<QDate>>(_variant).changeValue(value);
        break;
    case Type::Time:
        std::get<ValueTyped<QTime>>(_variant).changeValue(value);
        break;
    case Type::DateTime:
        std::get<ValueTyped<QDateTime>>(_variant).changeValue(value);
        break;
    case Type::BareDate:
        std::get<ValueTyped<BareDate>>(_variant).changeValue(value);
        break;
    case Type::Path:
        std::get<ValueTyped<Path>>(_variant).changeValue(value);
        break;
    case Type::VecBool:
        std::get<ValueTyped<QVector<bool>>>(_variant).changeValue(value);
        break;
    case Type::VecInt:
        std::get<ValueTyped<QVector<int>>>(_variant).changeValue(value);
        break;
    case Type::VecDouble:
        std::get<ValueTyped<QVector<double>>>(_variant).changeValue(value);
        break;
    case Type::VecString:
        std::get<ValueTyped<QVector<QString>>>(_variant).changeValue(value);
        break;
    case Type::VecDate:
        std::get<ValueTyped<QVector<QDate>>>(_variant).changeValue(value);
        break;
    case Type::VecTime:
        std::get<ValueTyped<QVector<QTime>>>(_variant).changeValue(value);
        break;
    case Type::VecDateTime:
        std::get<ValueTyped<QVector<QDateTime>>>(_variant).changeValue(value);
        break;
    case Type::VecBareDate:
        std::get<ValueTyped<QVector<BareDate>>>(_variant).changeValue(value);
        break;
    }
}

template <class U> void Value::changeValueAt(U value, int i)
{
    using std::get;
    switch(type()) {
    case Type::Uninitialized:
        ThrowException("Value is uninitialized");
        break;
    case Type::Null:
        ThrowException("Value is null");
        break;
    case Type::Bool:
    case Type::Int:
    case Type::Double:
    case Type::String:
    case Type::Date:
    case Type::Time:
    case Type::DateTime:
    case Type::BareDate:
    case Type::Path:
        if (i==0)
            changeValue(value);
        else
            ThrowException("Index out of range for a scalar").value(i);
        break;
    case Type::VecBool:
        get<ValueTyped<QVector<bool     >>>(_variant).ptr()[i] = convert<bool     >(value);
        break;
    case Type::VecInt:
        get<ValueTyped<QVector<int      >>>(_variant).ptr()[i] = convert<int      >(value);
        break;
    case Type::VecDouble:
        get<ValueTyped<QVector<double   >>>(_variant).ptr()[i] = convert<double   >(value);
        break;
    case Type::VecString:
        get<ValueTyped<QVector<QString  >>>(_variant).ptr()[i] = convert<QString  >(value);
        break;
    case Type::VecDate:
        get<ValueTyped<QVector<QDate    >>>(_variant).ptr()[i] = convert<QDate    >(value);
        break;
    case Type::VecTime:
        get<ValueTyped<QVector<QTime    >>>(_variant).ptr()[i] = convert<QTime    >(value);
        break;
    case Type::VecDateTime:
        get<ValueTyped<QVector<QDateTime>>>(_variant).ptr()[i] = convert<QDateTime>(value);
        break;
    case Type::VecBareDate:
        get<ValueTyped<QVector<BareDate >>>(_variant).ptr()[i] = convert<BareDate >(value);
        break;
    }
}


template <> const bool*               Value::constPtr() const;
template <> const int*                Value::constPtr() const;
template <> const double*             Value::constPtr() const;
template <> const QString*            Value::constPtr() const;
template <> const QDate*              Value::constPtr() const;
template <> const QTime*              Value::constPtr() const;
template <> const QDateTime*          Value::constPtr() const;
template <> const BareDate*           Value::constPtr() const;
template <> const Path*               Value::constPtr() const;
template <> const QVector<bool>*      Value::constPtr() const;
template <> const QVector<int>*       Value::constPtr() const;
template <> const QVector<double>*    Value::constPtr() const;
template <> const QVector<QString>*   Value::constPtr() const;
template <> const QVector<QDate>*     Value::constPtr() const;
template <> const QVector<QTime>*     Value::constPtr() const;
template <> const QVector<QDateTime>* Value::constPtr() const;
template <> const QVector<BareDate>*  Value::constPtr() const;

template <class U> U Value::as() const
{
    switch(type()) {
    case Type::Uninitialized:
//        ThrowException("Value is uninitialized");
        return U();
        break;
    case Type::Null:
        //ThrowException("Value is null");
        return U();
        break;
    case Type::Bool:
        return std::get<ValueTyped<bool>>(_variant).value<U>();
    case Type::Int:
        return std::get<ValueTyped<int>>(_variant).value<U>();
    case Type::Double:
        return std::get<ValueTyped<double>>(_variant).value<U>();
    case Type::String:
        return std::get<ValueTyped<QString>>(_variant).value<U>();
    case Type::Date:
        return std::get<ValueTyped<QDate>>(_variant).value<U>();
    case Type::Time:
        return std::get<ValueTyped<QTime>>(_variant).value<U>();
    case Type::DateTime:
        return std::get<ValueTyped<QDateTime>>(_variant).value<U>();
    case Type::BareDate:
        return std::get<ValueTyped<BareDate>>(_variant).value<U>();
    case Type::Path:
        return std::get<ValueTyped<Path>>(_variant).value<U>();
    case Type::VecBool:
        return std::get<ValueTyped<QVector<bool>>>(_variant).value<U>();
    case Type::VecInt:
        return std::get<ValueTyped<QVector<int>>>(_variant).value<U>();
    case Type::VecDouble:
        return std::get<ValueTyped<QVector<double>>>(_variant).value<U>();
    case Type::VecString:
        return std::get<ValueTyped<QVector<QString>>>(_variant).value<U>();
    case Type::VecDate:
        return std::get<ValueTyped<QVector<QDate>>>(_variant).value<U>();
    case Type::VecTime:
        return std::get<ValueTyped<QVector<QTime>>>(_variant).value<U>();
    case Type::VecDateTime:
        return std::get<ValueTyped<QVector<QDateTime>>>(_variant).value<U>();
    case Type::VecBareDate:
        return std::get<ValueTyped<QVector<BareDate>>>(_variant).value<U>();
    }
    return U();
}

// Needed for QMap<Value::Type>
inline uint qHash(const Value::Type &key) {
    return static_cast<uint>(key);
}

}

#endif
