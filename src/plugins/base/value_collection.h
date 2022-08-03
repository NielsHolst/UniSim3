/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_VALUE_COLLECTION_H
#define BASE_VALUE_COLLECTION_H

#include <variant>
#include <QDate>
#include <QDateTime>
#include <QTime>
#include "bare_date.h"
#include "exception.h"
#include "value.h"
//#include "value_typed.h"

namespace base {

class ValueCollection {
public:
    ValueCollection();
    ValueCollection(QVector<const Value*> values);
    ~ValueCollection();

    void initialize(QVector<const Value*> values);

    void update();
    // Update buffered values

    bool isEmpty() const;
    // Is collection empty?

    const Value& values();
    // Get values collected into one value

    Value::Type type() const;
    static Value::Type type(QVector<const Value*> values);
    // Common type of values; never a vector


private:
    // Pointers to original values
    QVector<const Value*> _original;

    // Vector buffer updated from _valuePtrs
    void *_buffer;
    void createBuffer(Value::Type type);

    template <class T>  QVector<T>* buf() {
        return static_cast<QVector<T>*>(_buffer);
    }

    // Holds return values
    Value _values;

    // _commonType contains the element type of the buffer
    Value::Type _commonType;

    // Promotion rules; in case of mixed base types
    static QVector<QSet<Value::Type>> _toInt, _toDouble, _toString;
};

}
#endif
