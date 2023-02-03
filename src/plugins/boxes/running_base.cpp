/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "running_base.h"

using namespace base;

namespace boxes {

RunningBase::RunningBase(QString name, Box *parent)
    : Box(name, parent)
{
    help("is a base class for running sum and average");
    Input(initial).help("Value at reset");
    Input(input).help("The input to sum or average");
    Output(value).help("Running sum or average");;
}

void RunningBase::reset() {
    if (bufferSize()<1)
        ThrowException("Buffer size < 1").value(bufferSize()).context(this);
    buffer.fill(initial, bufferSize());
    value = initial*bufferSize();
    pos = 0;
}

void RunningBase::update() {
    value += input - buffer.at(pos);
    buffer[pos] = input;
    pos = (pos+1)%bufferSize();
}

} //namespace

