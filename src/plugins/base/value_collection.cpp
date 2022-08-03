/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QSet>
#include <QStringList>
#include "convert.h"
#include "value_collection.h"

namespace base {

// Define subsets of compatible types and their destination type
using ValueSet  = QSet<Value::Type>;
using ValueSets = QVector<ValueSet>;
ValueSets
    ValueCollection::_toInt = ValueSets {
        ValueSet() << Value::Type::Bool << Value::Type::Int
    },
    ValueCollection::_toDouble = ValueSets {
        ValueSet() << Value::Type::Bool << Value::Type::Double,
        ValueSet() << Value::Type::Int << Value::Type::Double,
        ValueSet() << Value::Type::Bool << Value::Type::Int << Value::Type::Double
    };

ValueCollection::ValueCollection() : _buffer(nullptr) {
}

ValueCollection::ValueCollection(QVector<const Value*> values)
    : ValueCollection()
{
    initialize(values);
}

void ValueCollection::createBuffer(Value::Type type) {
    using Type = Value::Type;
    switch (type) {
    case Type::Bool         : _buffer = new vbool     ; break;
    case Type::Int          : _buffer = new vint      ; break;
    case Type::Double       : _buffer = new vdouble   ; break;
    case Type::String       : _buffer = new vQString  ; break;
    case Type::Date         : _buffer = new vQDate    ; break;
    case Type::Time         : _buffer = new vQTime    ; break;
    case Type::DateTime     : _buffer = new vQDateTime; break;
    case Type::BareDate     : _buffer = new vBareDate ; break;
    default: ThrowException("Illegal buffer type").value(Value::typeName(type));
    }
}

ValueCollection::~ValueCollection() {
    using Type = Value::Type;
    switch (_commonType) {
    case Type::Bool         : delete static_cast<vbool     *>(_buffer); break;
    case Type::Int          : delete static_cast<vint      *>(_buffer); break;
    case Type::Double       : delete static_cast<vdouble   *>(_buffer); break;
    case Type::String       : delete static_cast<vQString  *>(_buffer); break;
    case Type::Date         : delete static_cast<vQDate    *>(_buffer); break;
    case Type::Time         : delete static_cast<vQTime    *>(_buffer); break;
    case Type::DateTime     : delete static_cast<vQDateTime*>(_buffer); break;
    case Type::BareDate     : delete static_cast<vBareDate *>(_buffer); break;
    default: ;
    }
}

void ValueCollection::initialize(QVector<const Value*> _values) {
    if (!_original.isEmpty())
        ThrowException("Value collection can only be initialized once");
    if (_values.isEmpty())
        return;
    _original = _values;
    _commonType = type(_values);
    createBuffer(_commonType);
}

Value::Type ValueCollection::type() const {
    return _commonType;
}

Value::Type ValueCollection::type(QVector<const Value*> values) {
    using Type = Value::Type;
    QSet<Type> baseTypes;
    Type common;
    for (const Value *value : values) {
        if (!value->isNull())
            baseTypes << value->baseType();
    }

    if (baseTypes.size() == 0)
        common = Value::Type::Null;
    else if (baseTypes.size() == 1)
        common = *baseTypes.cbegin();
    else if (baseTypes == _toInt.at(0))
        common = Type::Int;
    else if (baseTypes == _toDouble.at(0) || baseTypes == _toDouble.at(1) || baseTypes == _toDouble.at(2))
        common = Type::Double;
    else
        common = Type::String;
    return common;
}

#define UPDATE(T) \
    buf<T>()->clear(); \
    for (auto v : _original) \
        *buf<T>() << v->as<T>(); \
    break

void ValueCollection::update() {
    using Type = Value::Type;
    switch (_commonType) {
    case Type::Uninitialized : ThrowException("Buffer is uninitialized");
    case Type::Bool          : UPDATE(bool     );
    case Type::Int           : UPDATE(int      );
    case Type::Double        : UPDATE(double   );
    case Type::String        : UPDATE(QString  );
    case Type::Date          : UPDATE(QDate    );
    case Type::Time          : UPDATE(QTime    );
    case Type::DateTime      : UPDATE(QDateTime);
    case Type::BareDate      : UPDATE(BareDate );
    default: ThrowException("Illegal common type").value(Value::typeName(_commonType));
    }
}

bool ValueCollection::isEmpty() const {
    return _original.isEmpty();
}

const Value& ValueCollection::values() {
    using Type = Value::Type;
    switch (_commonType) {
    case Type::Uninitialized : ThrowException("Buffer is uninitialized");
    case Type::Bool          : _values = buf<bool     >(); break;
    case Type::Int           : _values = buf<int      >(); break;
    case Type::Double        : _values = buf<double   >(); break;
    case Type::String        : _values = buf<QString  >(); break;
    case Type::Date          : _values = buf<QDate    >(); break;
    case Type::Time          : _values = buf<QTime    >(); break;
    case Type::DateTime      : _values = buf<QDateTime>(); break;
    case Type::BareDate      : _values = buf<BareDate >(); break;
    default: ThrowException("Illegal common type").value(Value::typeName(_commonType));
    }
    return _values;
}

}
