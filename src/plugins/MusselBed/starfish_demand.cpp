/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/

/* ##STARFISH GROWTH: calculates the total increment or decrease in biomass density for starfish as a function
   of food intake and temperature (respiration costs). Assimilatio function has been calculated from mesocosm experimentation
    respiration function yielded from the work by Mark Fonds, 1989 (unpublished) */

#include <base/exception.h>
#include <base/publish.h>
#include "starfish_demand.h"

using namespace base;

namespace MusselBed {

PUBLISH(StarfishDemand)

StarfishDemand::StarfishDemand(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(aGrowth).equals(0.181).help("Regression coefficient for growth demand");
    Input(bGrowth).equals(-0.516).help("Regression coefficient for growth demand");
    Input(aResp).equals(0.00161).help("Regression coefficient for respiration");
    Input(bResp).equals(0.103).help("Regression coefficient for respiration");
    Input(size).equals(5).help("starfish individual size at step");
    Input(biomass).equals(10).help("current starfish population biomass (g/m2)");
    Input(temperature).equals(13).help("current temperature");
    Output(netDemand).help("food demand to spend in growth g flesh/m2");
    Output(respiration).help("food needed to cover respiration/maintenance costs g flesh/m2");
}

void StarfishDemand::update() {

    netDemand = (0.11153368 - 0.021521*log(size))*biomass; /* Here the food demand required for growth under optimal conditions is calculated, this demand does not
                                                  include respiration costs, is all for growth*/
    respiration = (0.0004576*exp(0.2157*temperature))*biomass; /*Here the demand of food to cover for body maintenance costs */
//    netDemand = (size==0.) ? 0. : (aGrowth + bGrowth*log(size))*biomass;
//    if (netDemand < 0.) {
//        QString s = "a=%1; b=%2; size=%3, biomass=%4";
//        s = s.arg(aGrowth).arg(bGrowth).arg(size).arg(biomass);
//        ThrowException("Net growth demand < 0").value(netDemand).value2(s).context(this);
//    }
//    respiration = aResp*exp(bResp*temperature)*biomass;
}

} //namespace
