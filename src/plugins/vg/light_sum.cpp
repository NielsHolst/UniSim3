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
    help("accumulates indoors light sum");
    Input(numDays).equals(3.).unit("d").help("Number of days to accumulate light sum");
    Input(stepWithinDay).imports("calendar[stepWithinDay]");
    Input(timeStep).imports("calendar[timeStep]");
    Input(parIntensity).imports("plant[incidentPar]");
    Output(value).unit("mol/m2/d").help("Accumulated light sum");
}

void LightSum::reset() {
    _buffer.resize(numDays*24*3600/timeStep);
}

void LightSum::update() {
    _buffer.push(parIntensity*timeStep*1e-6);
    value = _buffer.sum()/numDays;
}

} //namespace

