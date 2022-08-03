#include <base/exception.h>
#include <base/publish.h>
#include "9_MinMaxValues.h"

using namespace base;

namespace cowpea {

PUBLISH(MinMaxValues)

MinMaxValues::MinMaxValues(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(input).help("flow from which you want to identify the minimum and maximum values");
    Input(date).help("current date based on calendar[dayOfYear]");

    Input(outflowTag).help("the tag on calendar scale at > 0 i.e. the calendar day the outflow starts a.k.a. last day of duration");
    Input(inflowTag).help("the tag on calendar scale at > 0 i.e. the calendar day the outflow starts a.k.a. last day of duration");
    Input(currentmaxvalue).help("current maximum value");
    Input(currentminvalue).help("current minimum value");
    Input(countinterval).help("rate of flow in question as in simulation step used for the flow e.g. hourly, daily, monthly etc.");
    Input(previousmaxvalue).help("previous maximum value based on a count interval i.e. currentmaxvalue - countinterval = previousmaxvalue");
    Input(previousminvalue).help("previous minimum value based on a count interval i.e. currentminvalue - countinterval = previousminvalue");

    Output(minvalue).help("minimum value found in the input flow");
    Output(maxvalue).help("maximum value found in the input flow");
    Output(counter).help("Current counter in sequence");
    Output(counterMax).help("Number of values in sequence");
}

void MinMaxValues::reset() {
    update();
}

void MinMaxValues::update() {

    previousmaxvalue = input;
    maxvalue = currentmaxvalue + previousmaxvalue;
    currentmaxvalue = maxvalue;

    if (outflowTag > 0) {
        maxvalue = 0;
    }




}


}
