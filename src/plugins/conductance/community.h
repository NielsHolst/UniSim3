/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef COMMUNITY
#define COMMUNITY
#include <QVector>
#include <base/box.h>
#include "phase.h"

namespace conductance {

class Plant;

class Community  : public base::Box
{
public:
    Community(QString name, QObject *parent);
	void initialize();
	void reset();
	void update();

    QVector<Plant*> plants;

private:
    // methods
    double sumZoneArea();
    void phaseUnlimitedChanged();
    void sortPlants();
    bool phaseUnderCompressionChanged();
    void updatePlants(double timeStep);
    void undoUpdatePlants();

    // state
    Phase phase;
    double _nextTimeStep;

    // inputs
    QVector<double> n, LA;

    // outputs
    double time, ZoneAreaTotal;

    // links
    Plant *smaller, *larger;

};

} //namespace
#endif
