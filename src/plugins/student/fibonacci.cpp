#include <base/publish.h>
#include "fibonacci.h"

using namespace base;

namespace student {

PUBLISH(Fibonacci)

Fibonacci::Fibonacci(QString name, Box *parent)
    : Box(name, parent)
{
    help("generates a Fibonacci sequence");
    Output(value);
}

void Fibonacci::reset() {
    previous = 0;
    value = 1;
}

void Fibonacci::update() {
    int value2 = value;
    value += previous;
    previous = value2;
}


}
