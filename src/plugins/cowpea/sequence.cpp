#include <base/exception.h>
#include <base/publish.h>
#include "sequence.h"

using namespace base;

namespace boxes {

PUBLISH(Sequence)

Sequence::Sequence(QString name, QObject *parent)
    : Box(name, parent)
{
    help("produces a sequence of numbers in a [min,max] range");
    Input(by).equals("update").help("The sequence value can either be updated at 'reset' or 'update'");
    Input(min).equals(0).help("Minimum value in sequence");
    Input(max).equals(1).help("Maximum value in sequence");

    Input(inflow).equals(1).help("Maximum value in sequence");


    Input(values).help("Vector of values; overrides min and max");
    Output(counter).help("Current counter in sequence");
    Output(counterMax).help("Number of values in sequence");;
    Output(offset).help("First value of counter, e.g. 0 or 1");
    Output(value).help("Current value in sequence");
}

void Sequence::amend() {
    if (by == "reset") {
        port("counter")->imports("/.[iteration]");
        port("counterMax")->imports("/.[iterations]");
        port("value")->noReset();
        offset = 1;
    }
    else if (by == "update") {
        port("counter")->imports("/.[step]");
        port("counterMax")->imports("/.[steps]");
        offset = 0;
    }
    else
        ThrowException("'by' must be either 'reset' or 'update'").value(by);
}

void Sequence::reset() {
    _useValues = !values.isEmpty();
    updateValue();
}

void Sequence::update() {
    updateValue();
}

void Sequence::updateValue() {
    if (counterMax < 1)
        ThrowException("'counterMax' must be > 0").value(counterMax).context(this);
    if (_useValues)
        value = values.at(counter-offset-1);
    else
        value = (counter == counterMax) ? max : min + (max - min + offset)*(counter - offset)/counterMax;


}


}
