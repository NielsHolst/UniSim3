/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include <base/test_num.h>
#include "light_sum.h"

using namespace base;
using namespace TestNum;

namespace vg {

PUBLISH(LightSum)

LightSum::LightSum(QString name, Box *parent)
    : Box(name, parent)
{
    help("accumulates indoors average light sum");
    Input(numDays).equals(3.).unit("d").help("Number of days to accumulate light sum");
    Input(stepWithinDay).imports("calendar[stepWithinDay]");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Input(daysPassed).imports("calendar[totalDays]");
    Input(totalPar).imports("budget[totalPar]");
    Input(sunPar).imports("budget[sunParHittingPlant]");
    Output(progressiveValue).unit("mol/m2/d").help("Value progressing during the day");
    Output(value).unit("mol/m2/d").help("Daily accumulated light sum averaged over `numDays`");
}

void LightSum::initialize() {
    _stepsPerDay = 24*3600/timeStepSecs;
}

void LightSum::reset() {
    _lightSum.resize(numDays*_stepsPerDay);
    _sunPar.resize(_stepsPerDay);
}

void LightSum::update() {        
    // Convert to mol PAR
    const double
        totalMolPar = totalPar*timeStepSecs*1e-6,
        sunMolPar   =   sunPar*timeStepSecs*1e-6;

    // Fill buffer at beginning
    if (eq(daysPassed, 1.)) {
        for (int i=0; i<numDays; ++i)
            fillInExpectedSunlight();
    }
    // At midnight
    else if (stepWithinDay == 0) {
        // Update achieved light sum at end of day
        value = progressiveValue;
        // Push todays's sunlight to light sum buffer; this is the expected total PAR for the coming day without growth light
        fillInExpectedSunlight();
        _lightSum.moveHead(-_stepsPerDay);
    }

    // Update progressive light sum
    _lightSum.push(totalMolPar);
    progressiveValue = _lightSum.sum()/numDays;

    // Update today's sunlight buffer
    _sunPar[stepWithinDay] = sunMolPar;

}

void LightSum::fillInExpectedSunlight() {
    for (double sunPar : _sunPar)
        _lightSum.push(sunPar);
}

} //namespace

