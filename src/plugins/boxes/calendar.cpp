/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
/* Day length and solar elevation equations copied from the FORTRAN code of Kroppf &
** Laar (1993). Modelling Crop-Weed Interactions. CAB International.
*/
#include <cfloat>
#include <cmath>
#include <QTextStream>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include <base/time_unit.h>
#include "calendar.h"

using namespace std;
using namespace base;
using namespace phys_math;

namespace boxes {

PUBLISH(Calendar)

Calendar::Calendar(QString name, Box *parent)
    : Box(name, parent)
{
    help("keeps check on date and time");
    Input(latitude).equals(52).help("Latitude").unit("[-90,90]");
    Input(longitude).equals(11).help("Longitiude").unit("[-180,180]");
    Input(timeZone).equals(1).help("Time zone").unit("h");
    Input(begin).equals(QDateTime(QDate(2000,1,1), QTime(0,0,0), Qt::UTC)).help("When simulation begins");
    Input(end).equals(QDateTime()).help("When simulation ends (optional)");
    Input(timeStep).equals(1).help("Time step in units of timeUnit").unit(">0");
    Input(timeUnit).equals("d").help("Unit of time step (y|d|h|m|s)");

    Output(steps).help("Number of steps from begin to end");
    Output(date).help("Current date");
    Output(time).help("Current time of the day");
    Output(dateTime).help("Current date and time");
    Output(timeStepSecs).help("Time step duration").unit("s");
    Output(timeStepDays).help("Time step duration").unit("d");
    Output(totalTimeSteps).help("Total number of time steps since calendar was reset");
    Output(totalTime).help("Total time since calendar was reset; in units of timeUnit").unit(port("timeUnit")->value<QString>());
    Output(totalDays).help("Total time since calendar was reset").unit("d");
    Output(dayOfYear).help("Julian day").unit("[1;366]");
    Output(bareDate).help("'date' as a bare date");
}

void Calendar::initialize() {
    // Some classes may need the outputs already in their initialize step
    reset();
}

void Calendar::reset() {
    if (timeStep < 1)
        ThrowException("Time step ('timeStep') must be larger than zero").value(timeStep);
    timeStepSecs = timeStep * Time::toSeconds(timeUnit);
    timeStepDays = timeStep * Time::toDays(timeUnit);
    dateTime = begin;
    if (end.isValid()) {
        if (begin >= end)
            ThrowException("Begin date must be before end data").
                    value(convert<QString>(begin)).value2(convert<QString>(end)).
                    context(this);
        auto totalSecs = begin.secsTo(end);
        steps = totalSecs/timeStepSecs;
    }
    else {
        steps = 1;
    }
    totalTimeSteps = 0;
    updateDerived();
}

void Calendar::update() {
    ++totalTimeSteps;
    dateTime = dateTime + Time::Period(timeStep, timeUnit);
    updateDerived();
}

void Calendar::updateDerived() {
    date = dateTime.date();
    time = dateTime.time();
    dayOfYear = date.dayOfYear();
    totalTime = totalTimeSteps*timeStep;
    totalDays = totalTime*Time::toDays(timeUnit);
    bareDate = BareDate(date);
}

} //namespace

