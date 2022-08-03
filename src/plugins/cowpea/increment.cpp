#include <base/publish.h>
#include "increment.h"


using namespace base;

namespace cowpea {

PUBLISH(Increment)

Increment::Increment(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(StartDate);
    Input(dayOfYear);
    Input(EndDate);
    Input(tMax);
    Input(increment);
    Input(Current);
    Output(value);
}

void Increment::reset() {

    update();
}

void Increment::update() {

    if (dayOfYear < StartDate) {
        value = 0;
    }

    else if (dayOfYear > EndDate) {

        value = 0;
    }

    else if (dayOfYear == StartDate) {

        value = Current + increment;
        Current = value;
    }






}


}


