/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/test_num.h>
#include "functional_response_g_b.h"

using namespace base;

namespace MusselBed {

PUBLISH(FunctionalResponseGB)

FunctionalResponseGB::FunctionalResponseGB(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(apparency).equals(1).help("The apparancy of the resource, also known as attack rate or search rate");
    Input(resourceDensity).help("The density of the resource (prey, host)");
    Input(demand).help("The finite demand rate of the consumer (predator, parasitoid) population");
    Input(egestionRatio).help("Egestion fraction of consumption");
    Input(respiration).help("The finite demand rate to cover respiration costs");
    Input(conversionCost).help("Proportion of the consumption going to conversion cost for converting prey to predator mass");
    Input(costOnRespiration).equals(true).help("Apply conversion cost also on respiration?");
    Input(truncateSupply).equals(false).help("If supply gets negative (i.e. too little is consumed to cover respiration) should it be truncated to zero?");

    Output(supply).help("The net supply obtained (net resource consumption, parasitoids infected). "
        "This will always be <= demand. If respiration exceeds consumption and truncateSupply is false, then supply will be negative.");
    Output(sdRatio).help("The supply/demand ratio [0;1]. This will be zero if supply is negative");
    Output(totalDemand).help("Total demand obtained to cover net supply, egestion and respiration");
    Output(totalSupply).help("Total supply obtained to cover net supply, egestion and respiration");
    Output(egestion).help("Amount of totalSupply lost to egestion");
    Output(attacksPerHost).help("Amount of totalSupply lost to egestion");
    Output(numHostsAttacked).help("Amount of totalSupply lost to egestion");
    Output(propHostsAttacked).help("Amount of totalSupply lost to egestion");
}

void FunctionalResponseGB::update() {
    if (egestionRatio < 0. || egestionRatio >= 1.)
        ThrowException("Illegal value for egestionRatio").value(egestionRatio).context(this);

    // Compute total supply (i.e., total kill)
    totalDemand = costOnRespiration ?
                (demand + respiration)/(1-conversionCost) :
                demand/(1-conversionCost) + respiration;
    totalDemand /= (1. - egestionRatio);
    totalSupply = f(totalDemand, apparency*resourceDensity);

    // Allocate total supply
    supply = totalSupply;
    egestion = supply*egestionRatio;
    supply -= egestion;
    supply -= costOnRespiration ? respiration/(1-conversionCost) : respiration;
    if (supply < 0.) {
        sdRatio = 0.;
        if (truncateSupply) supply = 0.;
    }
    else {
        sdRatio = divBounded(supply, demand, 1.);
        TestNum::snapToZero(sdRatio);
    }

    // Update host-parasitoid variables
    if (supply > 0) {
        attacksPerHost = resourceDensity > 0. ? supply/resourceDensity : 0.;
        propHostsAttacked = 1. - exp(-attacksPerHost);
        numHostsAttacked = resourceDensity*propHostsAttacked;
    }
    else {
        attacksPerHost =
        propHostsAttacked =
        numHostsAttacked = 0.;
    }
}

double FunctionalResponseGB::f(double demand, double resource) {
    return (demand==0.) ? 0. : demand*(1. - exp(-resource/demand));
}

// Safe floating point division with bound on result
double FunctionalResponseGB::divBounded(double x, double y, double bound) {
    if (y==0.)
        return bound;
    bool positive_xy = (x>0. && y>0.) || (x<0. && y<0.);
    if (positive_xy) {
        if (bound>0. && x/bound > y)
            return bound;
        else
            return x/y;
    }
    else {
        if (bound<0. && x/bound < y)
            return bound;
        else
            return x/y;
    }
}

} //namespace


