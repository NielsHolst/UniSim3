/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/environment.h>
#include <base/exception.h>
#include <base/random_order.h>
//#include "randomiser_stratified.h"
#include "random_base.h"
#include "randomiser_base.h"

#include <base/dialog.h>

using namespace base;

namespace boxes {

RandomBase::RandomBase(QString name, Box *parent)
    : Box(name, parent), _valueSet(false), _randomiser(nullptr)
{
    setClassName("boxes", "RandomBase");
    Input(P).equals(0.95).help("Range of the distribution covered by the [minValue;maxValue] range").unit("[0;1]");
    Input(drawAtInitialize).imports("ancestors::*/RandomiserBase::*[drawAtInitialize]");
    Input(drawAtReset).     imports("ancestors::*/RandomiserBase::*[drawAtReset]");
    Input(drawAtUpdate).    imports("ancestors::*/RandomiserBase::*[drawAtUpdate]");
}

void RandomBase::amend() {
    _number = new RandomOrder(1, this);
}

void RandomBase::initialize() {
    // Get number of strata
    _number->resize(randomiser()->numStrata());
    // Potential first draw
    if (drawAtInitialize && !_valueSet) {
        checkInputs("initialize");
        drawValue();
    }
    _valueSet = false;
}

void RandomBase::reset() {
    if (drawAtReset && !_valueSet) {
        checkInputs("reset");
        drawValue();
    }
    _valueSet = false;
}

void RandomBase::update() {
    if (drawAtUpdate && !_valueSet) {
        checkInputs("update");
        drawValue();
    }
    _valueSet = false;
}

RandomiserBase* RandomBase::randomiser() {
    if (!_randomiser) {
        auto boxes = findMany<RandomiserBase*>("*");
        switch (boxes.size()) {
        case 0:
            ThrowException("Boxscript must contain a Randomiser box").context(this);
            break;
        case 1:
            _randomiser = boxes[0];
            break;
        default:
            ThrowException("Boxscript must contain only one Randomiser box").
                    value("\n" + fullNames(boxes).join("\n")).context(this);
        }
    }
    return _randomiser;
}

} //namespace
