#include <base/publish.h>
#include "jump.h"

using namespace base;

namespace student {

PUBLISH(Jump)

Jump::Jump(QString name, Box *parent)
    : Box(name, parent)
{
    help("generates the 3n+1 sequence");
    Input(initial).equals(27);
    Output(value);
    Output(finished);
}

void Jump::reset() {
    value = (initial < 1) ? 1 : initial;
    finished = (value == 1);
}

void Jump::update() {
    value = (value%2 == 0) ? value/2 : 3*value + 1;
    finished = (value == 1);
}


}
