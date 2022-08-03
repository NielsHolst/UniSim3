/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SUN_H
#define SUN_H
#include <QTime>
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
    double latitude;
    int dayOfYear;
    QTime time;

    // outputs
    QTime sunrise, sunset;
    double dayLength,
        sinb, solarConstant, angot;
    bool isDay, isNight;
};

} //namespace
#endif
