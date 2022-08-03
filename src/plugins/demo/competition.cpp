#include <base/publish.h>
#include "competition.h"

using namespace base;

namespace student {

PUBLISH(Competition)

Competition::Competition(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(A);
    Output(value);
}

void Competition::reset() {
}

void Competition::update() {
    value = 0.;
    for (double a : A)
        value += a;
}


}
