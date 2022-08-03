#include <base/publish.h>
#include "li.h"

using namespace base;

namespace cowpea {

PUBLISH(LI)

LI::LI(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(k).help("extinction coefficient");
    Input(LAI).help("leaf are index");
    Input(radiation).help("PAR photosynthetically active radiation MJ.m-2.d-1");
    Input(startDate).help("interception starts at date");
    Input(endDate).help("interception ends date");
    Input(dayOfYear);



    Output(value).help("LI - light intercepted / APAR absorbed photosynthetically active radiation MJ/m2/d");
}

void LI::reset() {
    update();
}

void LI::update() {

    if (dayOfYear < startDate) {
        value = 0;
    }

    else if (dayOfYear > endDate) {

        value = 0;
    }

    else if (dayOfYear == startDate) {


    value = radiation * (1 - exp(-k * LAI));
    }


}

}
