/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_VALUE_TYPED_H
#define BASE_VALUE_TYPED_H

#include "convert.h"
#include "exception.h"
#include "path.h"

namespace base {

template <class T>
class ValueTyped {
public:
    ValueTyped()            : _variable(nullptr), _isInitialized(false) { }

    ValueTyped(T *variable) : ValueTyped() { initialize(variable); }

    ValueTyped(T value)     : ValueTyped() { initialize(value); }

    void initialize(T *variable)
    // Link to outside variable
    {
        if (_isInitialized)
            ThrowException("Value already initialized");
        _variable = variable;
        _isInitialized = true;
    }

    void initialize(T value)
    // Link to inside variable
    {
        if (_isInitialized)
            ThrowException("Value already initialized");
        _value = value;
        _isInitialized = true;
    }

    template <class U> void changeValue(U value)
    // Set to value with compatible type U
    {
        Q_ASSERT(_isInitialized);
        if (_variable)
            *_variable = convert<T>(value);
        else
            _value = convert<T>(value);
    }

    const T* constPtr() const
    // Return const pointer in native type T
    {
        Q_ASSERT(_isInitialized);
        return _variable ? _variable : &_value;
    }

    T* ptr()
    // Return pointer in native type T
    {
        Q_ASSERT(_isInitialized);
        return _variable ? _variable : &_value;
    }

    template <class U> U value() const
    // Get value in compatible type U
    {
        Q_ASSERT(_isInitialized);
        return convert<U>(_variable ? *_variable : _value);
    }

private:
    T _value;
    T* _variable;
    bool _isInitialized;
};

}

#endif
