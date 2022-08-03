/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <algorithm>
#include <base/exception.h>
#include <base/publish.h>
#include "phenology.h"

using namespace base;
using namespace std;

namespace coffee {

PUBLISH(Phenology)

Phenology::Phenology(QString name, QObject *parent)
    : Box(name, parent) {
    help("models coffee plant development");
    Input(juvenilePeriod).equals(900).unit("d").help("Period from planting until reproductive");
    Input(floweringOnsetTrigger).equals(1000.).unit("mm d").help("Trigger based on rainfall and day of year");
    Input(T0).equals(10.).unit("oC").help("Base temperature for development");
    Input(duration).equals(2780.).unit("DD").help("Duration of reproductive period");
    Input(daysSincePlanting).imports("sim[step]");
    Input(timeStep).imports("calendar[timeStepDays]");
    Input(rainfall).imports("weather[Rain]");
    Input(temperature).imports("weather[Tavg]");
    Input(date).imports("calendar[date]");
    Output(growthStage).unit("[0;1]").help("Relative development into reproductive stage");
    Output(isWaitingForRain).unit("bool").help("Is the plant waiting for rain, to turn reproductive?");
    Output(isReproductive).unit("bool").help("Is the plant in the reproductive stage?");
    Output(isHarvestTime).unit("bool").help("Is it harvest time now?");
}

void Phenology::reset() {
    isWaitingForRain = true;
    isReproductive = false;
    isHarvestTime = false;
    growthStage = 0.;
}

void Phenology::update() {
    isHarvestTime = false;
    isWaitingForRain = isWaitingForRain || date.dayOfYear()==365;
    double rainIndex = rainfall*date.dayOfYear();
    isReproductive = isReproductive ||
           (daysSincePlanting>=juvenilePeriod &&
            isWaitingForRain &&
            rainIndex>floweringOnsetTrigger);

    if (isReproductive) {
        double devRate = max(0., temperature-T0)/duration;
        growthStage += devRate;
        if (growthStage > 1.) {
            isWaitingForRain = false;
            isReproductive = false;
            isHarvestTime = true;
            growthStage = 0.;
        }
    }
}

} //namespace

