/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SUN_H
#define SUN_H
#include <QDateTime>
#include <base/box.h>

namespace boxes {

class Sun : public base::Box
{
public: 
    Sun(QString name, Box *parent);
    void reset();
	void update();

private:
    // inputs
    double latitude, longitude, timeZone, altitude;
    QDateTime dateTime;

    // outputs
    QTime sunrise, noon, sunset, solarTime;
    double dayLength, elevation, azimuth,
        solarConstant, angot;
    bool isDay, isNight;
};

} //namespace
#endif
