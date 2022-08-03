/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CALENDAR_H
#define CALENDAR_H
#include <QDate>
#include <QDateTime>
#include <QTime>
#include <base/box.h>

namespace boxes {

class Calendar : public base::Box
{
public: 
    Calendar(QString name, Box *parent);
	//standard methods
    void initialize();
    void reset();
	void update();

private:
    // inputs
    double latitude, longitude;
    QDateTime begin, end;
    int timeStep, timeZone;
    QString timeUnit;

    // outputs
    int steps;
    QDate date;
    QTime time;
    QDateTime dateTime;
    int dayOfYear, totalTime, totalTimeSteps;
    double timeStepSecs, timeStepDays, totalDays;

    // methods
    void updateDerived();
};

} //namespace
#endif
