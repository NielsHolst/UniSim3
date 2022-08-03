/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Isaac Kwesi Abuley , Aarhus University [ikabuley at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "PTmax.h"
#include "math.h"
#include "QtMath"

using namespace base;

namespace student {

PUBLISH(PhysiologicDevMaxTemp)
PhysiologicDevMaxTemp::PhysiologicDevMaxTemp(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(Tmax).imports("weather[Tmax]").help("The maximum temperature from the weather data");
    Input(Tfloor).equals(7.).help("The base temperature threshold, if not specified then the default is 7C");
    Input(Tceiling).equals(30.).help("The maximum temperature threshold, if not sepecified then the default is 30C");
    Input(Topt).equals(21.).help("The optimum temperature threshold, if not specified then the defualt is 21C");
    Output(PTmax).help ("The Physiologic Development at the minimum temperature ina day");
}

void PhysiologicDevMaxTemp::reset() {
   update();
}

void PhysiologicDevMaxTemp::update() {
    if (Tmax < Tfloor || Tmax >= Tceiling)
         {PTmax= (0.);}
    else if (Tmax >= Tfloor && Tmax < Topt)
        {PTmax = 10. * (1. - (pow((Tmax - Topt),2.) / (pow((Topt - Tfloor),2.))));}
    else if (Tmax >= Topt && Tmax < Tceiling)
        {PTmax = 10. * (1. - ((pow((Tmax - Topt), 2.)) / (pow((Tceiling - Topt),2.))));}
}

}
