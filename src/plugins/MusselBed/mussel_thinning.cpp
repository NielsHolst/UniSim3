/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/

/* ##MUSSEL POPULATION SELF-THINNING RULE: estimates losses due to increase of individual size
   Thinning. This equation have been calculated based on field observations in the Wadden Sea.
   thinning occur when the density in numbers/m2 is too high for the current average mussel individual
   size. This model evaluates the population after it has grown for the day, and then stablish a maximum
   number of mussel/m2. The difference with the current density numbers is then eliminated from the bed.
   However as starfish predation can have a thinning effect, this thinning value is compared with starfish
   predation for the current date and the bigger one is used to determine the density loss for the mussel bed*/

#include <math.h>
#include <base/publish.h>
#include "mussel_thinning.h"


using namespace base;

namespace MusselBed {

PUBLISH(MusselThinning)

MusselThinning::MusselThinning(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(a).equals(3300.).help("Regression coefficient");
    Input(b).equals(-0.871).help("Regression coefficient");
    Input(biomass).equals(1).help("Mussel density in biomass").unit("g/m2");
    Input(abundance).equals(1).help("Mussel density in numbers").unit("m-2");;
    Input(size).equals(1).help("Mussel size").unit("g");;
    Input(supply).help("Biomass of mussels consumed by starfish").unit("g/m2");;
    Output(maxAbundance).help("Maximum density of mussels for the current mussel size").unit("m-2");
    Output(lossAbundance).help("Mussel loss in numbers").unit("m-2");
    Output(lossBiomass).help("Mussel loss in biomass").unit("g/m2");
}

void MusselThinning::reset() {
    update();
}

void MusselThinning::update() {
        maxAbundance = a*pow(size, b);
        double thinning = (maxAbundance - abundance)* size; /*this compare the max number with the current abundance and calculate the amount of mussels
                                                            that died in this step due to intraspecific competition */

        if (thinning<0 && (-thinning)>=supply){ /* Thinning is then evaluated against the amount of mussels consumed by starfish, if thinning is larger, then
                                                    thinning will determine the decrease of mussel biomass and numbers in the step instead of starfish consumed mussels*/
            lossBiomass = - thinning;
            lossAbundance = - thinning/size;
        }
        else {
            lossBiomass = supply;
            lossAbundance = supply/size;
        }
}
} //namespace

