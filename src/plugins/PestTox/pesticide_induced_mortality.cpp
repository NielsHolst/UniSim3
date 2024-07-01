/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include "pesticide_induced_mortality.h"
#include <cmath>
#include <base/exception.h>
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (PesticideInducedMortality)
	
PesticideInducedMortality::PesticideInducedMortality(QString name, Box *parent)
	: Box(name, parent)
{

    help("manages pesticide-induced mortality rate");
    Input(LC50).unit("mg a.i/L").equals(0.).help("Lethal concentration that kills 50% of the population");
    Input(concentration).unit("mg a.i/L").equals(0.).help("Current pesticide concentration in the water");
    Input(slope).unit("volume/amount").equals(1.).help("Scale the specific toxicity of the simulated pesticide or sigmoidal function");

    Output(mortalityRate).unit("fraction/day").help("Mortality rate");
    Output(survivalRate).unit("fraction/day").help("Survival rates");
}

void PesticideInducedMortality::reset() {
    mortalityRate = 0.;
    survivalRate = 1.;
    if (LC50<=0.)
        ThrowException("LC50 must be positive").value(LC50).context(this);
}

void PesticideInducedMortality::update() {
    mortalityRate = (concentration > 0.) ? 1./(1. + exp(-slope*(log(concentration) - log(LC50)))) : 0.;
    survivalRate = (survivalRate < 0) ? 0 : (1.- mortalityRate);
}

} //namespace

