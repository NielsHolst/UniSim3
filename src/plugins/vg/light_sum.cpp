/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "light_sum.h"

using namespace base;

namespace vg {

PUBLISH(LightSum)

LightSum::LightSum(QString name, Box *parent)
    : Box(name, parent)
{
    help("accumulates indoors average light sum");
    Input(numDays).equals(3.).unit("d").help("Number of days to accumulate light sum");
    Input(stepWithinDay).imports("calendar[stepWithinDay]");
    Input(timeStepSecs).imports("calendar[timeStepSecs]");
    Input(indoorsPar).imports("plant[incidentPar]");
    Input(outdoorsPar).imports("outdoors[par]");
    Input(atMidnight).imports("calendar[atMidnight]");
    Output(value).unit("mol/m2/d").help("Accumulated light sum averages over `numDays`");
    Output(indoorsAccumulated).unit("mol/m2").help("Today's accumulated indoors light");
    Output(outdoorsExpected).unit("mol/m2").help("Expected sunlight sum for the rest of the day");
}

void LightSum::initialize() {
    _stepsPerDay = 24*3600/timeStepSecs;
    _buffer[0].resize(_stepsPerDay);
    _buffer[1].resize(_stepsPerDay);
    _yesterday = 0;
    _today = 1;
}

void LightSum::reset() {
    _lightSum.resize(numDays*_stepsPerDay);
    indoorsAccumulated = 0.;
}

void LightSum::update() {
    // Update indoors light sum buffer
    const double indoorsMolPar = indoorsPar*timeStepSecs*1e-6;
    _lightSum.push(indoorsMolPar);
    value = _lightSum.sum()/numDays;
    // At midnight
    if (stepWithinDay == 0) {
        // Swap buffers
        _yesterday = 1 - _yesterday;
        _today     = 1 - _today;
        // Update yesterday's buffer to contain summed PAR until midnight
        double sum = 0;
        for (auto it = _buffer[_yesterday].rbegin(); it != _buffer[_yesterday].rend(); ++it) {
            sum += *it;
            *it = sum;
        }
    }
    // Accumulate today's indoors light
    if (atMidnight)
        indoorsAccumulated = indoorsMolPar;
    else
        indoorsAccumulated += indoorsMolPar;
    // Update today's outdoors light buffer
    _buffer[_today][stepWithinDay] = outdoorsPar*timeStepSecs*1e-6;;
    outdoorsExpected = _buffer[_yesterday].at(stepWithinDay);
}

} //namespace

