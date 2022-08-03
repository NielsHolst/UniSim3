/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <algorithm>
#include <functional>
#include <base/publish.h>
#include "buffer.h"

using namespace std;
using namespace base;

namespace boxes {

PUBLISH(Buffer)

Buffer::Buffer(QString name, Box *parent)
    : Box(name, parent)
{
    help("is a first in-first out buffer");
    Input(initial).help("Initial amount at first place in buffer");
    Input(input).help("Input to next place in buffer");
    Input(increment).help("Increment added to every place in buffer");
    Input(size).equals(100).help("Number of places in buffer");
    Output(sum).help("Total amount in buffer");
    Output(average).help("Average amount of places used in buffer");
    Output(buffer).help("The buffer itself");
}

void Buffer::reset() {
    sum = average = 0;
    if (size < 1) size = 1;
    buffer.fill(0, size);
    _buffer.set_capacity(size);
    _firstUpdate = true;
}

void Buffer::update() {
    double totalInput = input;
    if (_firstUpdate) {
        totalInput += initial;
        _firstUpdate = false;
    }
    double take = _buffer.full() ? _buffer.front() : 0;
    _buffer.push_back(totalInput);
    if (increment != 0)
        transform(_buffer.begin(), _buffer.end(), _buffer.begin(),
                  [this](double value) { return value + this->increment; });
    int n = _buffer.size();
    sum += totalInput + n*increment- take;
    average = sum/n;
    updateOutputBuffer();
}

void Buffer::updateOutputBuffer() {
    auto range1 = _buffer.array_one(),
         range2 = _buffer.array_two();
    int n1 = range1.second,
        n2 = range2.second;
    const double *source1 = range1.first,
                 *source2 = range2.first;
    double *dest1 = buffer.data();
    copy(source1, source1 + n1, dest1);
    copy(source2, source2 + n2, dest1 + n1);
}

}
