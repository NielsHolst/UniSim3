/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RANDOM_BASE_TYPED_H
#define RANDOM_BASE_TYPED_H
#include <base/environment.h>
#include <base/exception.h>
#include "random_base.h"

namespace boxes {

template <class T> class RandomBaseTyped : public RandomBase
{
public: 
    RandomBaseTyped(QString name, Box *parent);
    using RandomBase::update;
    void update(base::Port *port);
    void setValue(double newValue);
    double getValue() const;

protected:
    // Inputs
    T min, max;
    // Outputs
    T value;
    void checkInputs(QString text);
};

template <class T>
RandomBaseTyped<T>::RandomBaseTyped(QString name, Box *parent)
    : RandomBase(name, parent)
{
    Input(min).equals(static_cast<T>(0)).help("Minimum value (included)");
    Input(max).equals(static_cast<T>(1)).help("Maximum value (excluded)");
    Output(value).noClear().help("The most recently drawn value");
}

template <class T>
void RandomBaseTyped<T>::checkInputs(QString text) {
    if (min >= max)
        ThrowException(text + ": Boundaries must obey min<max").value(min).value2(max).context(this);
}

template <class T>
void RandomBaseTyped<T>::update(base::Port *port) {
    drawValue();
    port->equals(value);
}

template <class T>
void RandomBaseTyped<T>::setValue(double newValue) {
    value = static_cast<T>(newValue);
    _valueSet = true;
}

template <class T>
double RandomBaseTyped<T>::getValue() const {
    return static_cast<T>(value);
}

} //namespace
#endif
