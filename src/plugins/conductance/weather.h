/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef WEATHER
#define WEATHER
#include <base/box.h>

namespace conductance {

class Weather : public base::Box
{
public:
    Weather(QString name, QObject *parent);
private:
    // input
    double irradiation;
};

} //namespace
#endif
