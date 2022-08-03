/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Isaac Kwesi Abuley , Aarhus University [ikabuley at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "PTmin.h"
#include "math.h"
#include "QtMath"

using namespace base;

namespace student {

PUBLISH(PhysiologicDevMinTemp)
PhysiologicDevMinTemp::PhysiologicDevMinTemp(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(Tmin).imports("weather[Tmnin]").help("The minimum temperature from the weather data");
    Input(Tfloor).equals(7.).help("The base temperature threshold, if not specified then the default is 7C");
    Input(Tceiling).equals(30.).help("The maximum temperature threshold, if not sepecified then the default is 30C");
    Input(Topt).equals(21.).help("The optimum temperature threshold, if not specified then the defualt is 21C");
    Output(PTmin).help("The Physiologic Development at the minimum temperature ina day");
}

void PhysiologicDevMinTemp::reset() {
   update();
}

void PhysiologicDevMinTemp::update() {
    if (Tmin < Tfloor || Tmin >= Tceiling)
         {PTmin= (0.);}
    else if (Tmin >= Tfloor && Tmin < Topt)
        {PTmin = 10. * (1. - (pow((Tmin - Topt),2.) / (pow((Topt - Tfloor),2.))));}
    else if (Tmin >= Topt && Tmin < Tceiling)
        {PTmin = 10. * (1. - ((pow((Tmin - Topt), 2.)) / (pow((Tceiling - Topt),2.))));}
}


}
