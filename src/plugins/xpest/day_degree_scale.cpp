/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "day_degree_scale.h"
#include <base/publish.h>

using namespace base;

namespace xpest {
	
PUBLISH(DayDegreeScale)

DayDegreeScale::DayDegreeScale(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(temperature).imports("weather[Tavg]");
    Input(cropGrowthStage).imports("..[growthStage]");
    Input(from).help("Initial growth stage");
    Input(to).equals(1.).help("Final growth stage");
    Input(T0).help("Lower threshold for development").unit("oC");
    Input(duration).help("Duration of growth stage").unit("DD");

    Output(growthStage).help("Growth stage");
    Output(completed).help("Has stage been completed?");
}

void DayDegreeScale::reset() {
    _age = 0.;
    completed = false;
    updateFromAge();
}

void DayDegreeScale::update() {
    if (!completed && cropGrowthStage >= from && cropGrowthStage < to) {
        double dT = temperature - T0;
        if (dT > 0.) {
            _age += dT/duration;
            if (_age > 1.) _age = 1.;
            updateFromAge();
        }
    }
}

void DayDegreeScale::updateFromAge() {
    growthStage = from + _age*(to - from);
    completed = (_age == 1.);
}

} //namespace

