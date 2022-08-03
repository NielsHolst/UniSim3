/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/any_year.h>
#include <base/exception.h>
#include <base/interpolate.h>
#include <base/publish.h>
#include "crop.h"
#include "weather.h"

using namespace base;

namespace awe {

PUBLISH(Crop)

Crop::Crop(QString name, QObject *parent)
    : Box(name,parent)
{
    Input(currentCrop).imports("..[currentCrop]");
    Input(nextCrop).imports("..[nextCrop]");
    Input(date).imports("calendar[date]");
    Input(sowingDate).equals("1/4/*").help("Date of sowing");
    Input(harvestDate).equals("1/8/*").help("Date of harvest");
    Input(weedExchangeRate).equals(0.6).help("Final biomass of weeds as a proportion of yield loss (g/g)");
    Input(maxYield).equals(800.).help("Weed-free yield (g/m @Sup {2})");
    Input(maxYieldLossPct).equals(60.).help("Maximum yield loss (%) caused by weeds (Cousens's @I {a})");
    Input(slopeYieldLossPct).equals(0.5).help("Initial yield loss per weed (% per weed density equivalent) caused by weeds (Cousens's @I {i})");
    Input(laiX).help("Time after harvest");
    Input(laiY).help("Corresponding LAI");
    Input(ddSum).help("Day-degrees after sowing");
    Output(lai).help("Leaf area index (m @Sup {2}/m @Sup {2}).");
    Output(sowToday).help("Is the crop sowed today?");
    Output(harvestToday).help("Is the crop sowed today?");
    Output(isSown);
    Output(isHarvested);
}

void Crop::initialize() {
    if (laiX.size() != laiY.size()) {
        QString msg{"laiX (%1) and laiY (%2) must be same length"};
        ThrowException(msg.arg(laiX.size()).arg(laiY.size()));
    }
    int n = laiX.size();
    for (int i=0; i<n; ++i)
        laiXY[laiX.at(i)] = laiY.at(i);
}

void Crop::reset() {
    lai = 0.;
    isSown = isHarvested = sowToday = harvestToday = false;
}

void Crop::update() {
    bool isCurrent = (currentCrop == objectName()),
         isNext = (nextCrop == objectName());
    sowToday = isNext && equals(date, sowingDate);
    if (sowToday) {
        isSown = true;
        isHarvested = false;
    }
    harvestToday = isCurrent && equals(date, harvestDate);
    if (harvestToday) {
        isSown = false;
        isHarvested = true;
    }
    lai = isCurrent && isSown && !sowToday  ? interpolateLai() : 0.;
}

double Crop::interpolateLai() const {
    return interpolate(laiXY, ddSum);
}

}
