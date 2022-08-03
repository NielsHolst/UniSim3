/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef WEATHER_H
#define WEATHER_H

#include <base/box.h>

namespace awe {

class Weather : public base::Box
{
public:
    Weather(QString name, QObject *parent=0);
    void reset();
	void update();
private:
    // inputs
    double Tmin, max, c;
    int offset, dayOfYear;
    // outputs
    double T;
    // methods
    double temperature(int dayOfYear) const;
};


}
#endif
