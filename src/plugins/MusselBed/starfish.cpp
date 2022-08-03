/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/

/* ## STARFISH POPULATION MODEL: actualize starfish population density at each step, considering environmentally scalated supply and population maintenance costs.
      Due to how the FunctionalResponseGB plugin works, this function uses netSupply that includes total consumed flesh. When consumed flesh is 0 or smaller than
      respiration costs, starfish shrink at rate determined by respiration*/

#include <math.h>
#include <base/publish.h>
#include "starfish.h"

using namespace base;

namespace MusselBed {

PUBLISH(Starfish)

Starfish::Starfish(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(initBiomass).help("Starfish density at t0 (g/m2");
    Input(initAbundance).help("Starfish density at t0 in numbers");
    Input(conversionEfficiency).help("Efficiency turning mussel prey into starfish biomass").unit("[0;1]");
    Input(supply).help("consumed food under current environmental condtions this supply have to be used for respiration and growth");
    Input(respiration).help("food necessary to cover for maintenance costs");
    Output(abundance).help("starfish density in numbers");
    Output(biomass).help("current step starfish density (g/m2)");
    Output(size).help("current average individual size (g)");
}
void Starfish::reset() {
    biomass = initBiomass;
    abundance = initAbundance;
    size=initBiomass/initAbundance;
//    update();
}

void Starfish::update() {
     /*this function calculates the growth rate as a conversion factor of the food available after covering
       maintenance costs. Growth rate can be negative if supply doesn't cover maintenance costs, in that case
       starfish population shrink in biomass not in numbers */
    double growthrate = conversionEfficiency*((supply) - respiration);

    // growth rate can be negative, although respiration costs tend to zero as biomass decrease, to ensure
    // apropiate model behavior, this function avoid negative densities.
    if (growthrate<0 && fabs(growthrate)>biomass) {
        biomass = 0;
    }
    else {
        biomass = growthrate+biomass;
    }

    // abundance as number of individuals is constant, this model does not incorporate predator mortality or migration, etc. this
    //   part updates individual size considering the increase in biomass.
    size = abundance > 0 ? biomass/abundance : 0;

}

} //namespace

