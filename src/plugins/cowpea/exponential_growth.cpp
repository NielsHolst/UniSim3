#include <base/publish.h>
#include "exponential_growth.h"


using namespace base;

namespace cowpea {

PUBLISH(ExponentialGrowth)

ExponentialGrowth::ExponentialGrowth(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(a).help("value at the start");
    Input(k).help("growth rate");
    Input(t).help("time i.e. growth at day 1, growth at day 2...");

    Output(value).help("value at time t");
}

void ExponentialGrowth::reset() {

    update();
}

void ExponentialGrowth::update() {

    value = a * exp(k * t);
}




}
