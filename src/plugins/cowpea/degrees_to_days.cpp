#include <base/publish.h>
#include "degrees_to_days.h"

using namespace base;

namespace cowpea {

PUBLISH(degreestodays)

degreestodays::degreestodays(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(DD).help("current DayDegrees[step]");
    Input(currentDate).help("current date");
    Input(T0).help("base temperature");
    Input(Tmax).help("maximum temperature");
    Input(plusday).help("amount by which the starting value will be increasing daily(steply)");
    Input(startvalue).help("maximum temperature");
    Input(inflow).help("the inflow or whatever sequence/array of values you need to tag at >0");
    Input(position).help("a scale of choice e.g. date");
    Input(outflow).help("the outflow or whatever sequence/array of values you need to tag at >0");



    Output(value).help("count of calendar days when inflowTag is > outflowTag i.e. duration from inflow to outflow");
    Output(counterMax).help("Number of values in sequence");
    Output(inflowTag).help("the tag on calendar scale at >0 i.e. the calendar day the inflow starts");
    Output(outflowTag).help("the tag on calendar scale at >0 i.e. the calendary day the outflow starts");
}

void degreestodays::reset() {
    update();
}

void degreestodays::update() {

    if (inflow > 0) {
        inflowTag = position;
    }

    if (outflow > 0) {
        outflowTag = position;
    }

    if (inflowTag > outflowTag) {
        value = startvalue + plusday;
        startvalue = value;
    }

    if (inflowTag == outflowTag) {
        value = 0;
    }







    if (DD > 0) {
        value = startvalue + plusday;
        startvalue = value;
    }

    else if (DD == 0.) {

        value = 0;
    }




}


}
