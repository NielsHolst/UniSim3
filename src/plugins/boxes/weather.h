/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef WEATHER_H
#define WEATHER_H
#include <base/box.h>

namespace boxes {

class Weather : public base::Box
{
public:
    Weather(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double Tmin, Tmax;
    // Outputs
    double Tavg;
};

}

#endif
