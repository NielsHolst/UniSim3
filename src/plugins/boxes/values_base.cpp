/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "combinations.h"
#include "values_base.h"

using namespace base;

namespace boxes {

ValuesBase::ValuesBase(QString name, Box *parent)
    : Box(name, parent)
{
    Input(counter).help("Current counter into values");
    Output(numValues).noClear().help("Number of values");
    Output(values).noClear().help("Vector of all values");
    Output(value).noClear().help("Current value");
}

void ValuesBase::amend() {
    setCounter();
}

void ValuesBase::initialize() {
    setValues();
    setIterations();
    setSteps();
}

void ValuesBase::setCounterByIteration() {
    QString s = parent<Box*>()->className();
    if (s.contains("Combination")) {
        QString name = Combinations::counterName(this);
        port("counter")->imports("..[" + name + "]");
        _offset = 0;
    }
    else {
        port("counter")->imports("/.[iteration]");
        _offset = 1;
    }
}

void ValuesBase::setCounterByStep() {
    port("counter")->imports("/.[step]");
}

void ValuesBase::resetByIteration() {
    int index = (counter - _offset)%numValues;
    value = values.at(index);
}

void ValuesBase::resetByStep() {
    setSteps();
    update();
}

void ValuesBase::updateByStep() {
    // step counts from 0
    int index = counter%numValues;
    value = values.at(index);
}

}
