#include <base/publish.h>
#include "biomass.h"

using namespace base;

namespace cowpea {

PUBLISH(Biomass)

Biomass::Biomass(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(LUE).help("light use efficiency in g/MJ");
    Input(LightIntercepted).help("light energy intercepted MJ/m2/d");
    Input(duration).help("growing season in days");
    Input(BiomassIncrementPrevious).help("amount of daily biomass increase from a previous day in g/m2/d");
    Input(startDate).help("interception starts at date");
    Input(endDate).help("interception ends date");
    Input(dayOfYear).help("for using calendar[dayOfYear] as a base time scale");


    Output(value).help("accrued g/m2/d");
    Output(BiomassIncrementCurrent).help("amount of daily biomass increase for a current date day in g/m2/d");

}

void Biomass::reset() {
    update();
}

void Biomass::update() {

    BiomassIncrementPrevious = value;

    BiomassIncrementCurrent = LUE*LightIntercepted;

    value = BiomassIncrementCurrent + BiomassIncrementPrevious;

    if (dayOfYear < startDate) {
        value = 0;
    }

    else if (dayOfYear > endDate) {

        value = 0;
    }






}


}
