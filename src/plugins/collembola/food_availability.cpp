/* Copyright 2025-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Liyan Xie, Aarhus University [liyan.xie at agro.au.dk] and 
** Anne Dahl Svendsen, Aarhus University [anne.svendsen at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include "food_availability.h"

using namespace base;

namespace collembola {

PUBLISH(FoodAvailability)

FoodAvailability::FoodAvailability(QString name, Box *parent)
    : Box(name, parent)
{
    Input(temperature).unit("oC").imports("weather[Tavg]").help("Soil or ambient temperature?");
    Input(soilMoisture).unit("MPa").help("Not yet implemented");
    Input(Tmin).unit("oC").equals(5.);
    Input(Topt).unit("oC").equals(20.);
    Input(Tmax).unit("oC").equals(30.);
    Input(ATmin).unit("-").equals(0.2);
    Input(ATbetween).unit("-").equals(0.6);
    Input(ATmax).unit("-").equals(1.0);
    Output(value).unit("-").help("Food availability index");
}

void FoodAvailability::reset() {
    update();
}

void FoodAvailability::update() {
    if (temperature < Tmin)
        value = ATmin;
    else if (temperature < Topt)
        value = ATbetween;
    else if (temperature < Tmax)
        value = ATmax;
    else
        value = Tmin;
}

} //namespace

