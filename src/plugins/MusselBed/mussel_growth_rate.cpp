/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/

/* ## MUSSEL GROWTH MODEL: calculate an increase of biomass for the mussel population, based in an
   intrinsic (optimum) growth rate that is scalated by current temperature and salinity at step*/
#include <base/publish.h>
#include "mussel_growth_rate.h"

using namespace base;

namespace MusselBed {

PUBLISH(MusselGrowthRate)

MusselGrowthRate::MusselGrowthRate(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(a).equals(0.0159).help("Regression coefficient");
    Input(b).equals(-0.486).help("Regression coefficient");
    Input(carryingCapacity).equals(15000).help("Carrying capacity (g/m2), obtained from field observations");
    Input(biomass).equals(3000).help("current mussel density at step(g/m2)");
    Input(size).equals(1).help("average individual size in g");
    Output(value).help("maximum rate of increase considering mussel average size (% per day gr/gr)");
}

void MusselGrowthRate::initialize() {
    scales = findMany<Box*>("scales/*");
}

void MusselGrowthRate::reset() {
    update();
}

void MusselGrowthRate::update() {
//    value = (0.0197*pow(size,-0.232));
    value = a*pow(size,b);
    for (int i = 0; i < scales.size(); ++i) {
        value *= scales[i]->port("value")->value<double>();
    }

    value = value*biomass*(1.-(biomass/carryingCapacity));
}


} //namespace

