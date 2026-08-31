/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef CALENDAR_H
#define CALENDAR_H

#include <QDate>
#include <QDateTime>
#include <QTime>

#include <base/bare_date.h>
#include <base/box.h>
#include <base/base_dllexport.h>

namespace boxes {

class UNI_API Calendar : public base::Box
{
public: 
    Calendar(QString name, Box *parent);
	//standard methods
    void initialize() override;
    void reset() override;
    void update() override;

private:
    // inputs
    double latitude, longitude;
    QDateTime begin, end;
    int timeStep, timeZone;
    QString timeUnit;

    // outputs
    int steps, stepWithinDay, stepsPerDay;
    QDate date;
    QTime time;
    QDateTime dateTime;
    bool atMidnight;
    int dayOfYear, totalTime, totalTimeSteps;
    double timeStepSecs, timeStepDays, totalDays;
    base::BareDate bareDate;

    // methods
    void updateDerived();
};

} //namespace
#endif
