#include <base/publish.h>
#include "daily_larval_mortality.h"

using namespace base;

namespace cowpea {

PUBLISH(DailyLarvalMortality)

DailyLarvalMortality::DailyLarvalMortality(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(sdRatio).unit("[0;1]").help("Supply/Demand ratio");
    Input(exponent).equals(0.5).unit(">0").help("Concave (<1), linear (1), or convex (>1) response");
    Input(timeStep).imports("calendar[timeStepDays]");
    Output(value).unit("[0;1]").help("Daily survival rate");
}

void DailyLarvalMortality::reset() {
    update();
}

void DailyLarvalMortality::update() {
    value = (sdRatio < 1.) ? pow(1. - sdRatio, exponent)*timeStep : 0. ;
}

}
