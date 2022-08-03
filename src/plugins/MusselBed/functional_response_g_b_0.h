/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FUNCTIONAL_RESPONSE_G_B
#define FUNCTIONAL_RESPONSE_G_B

#include <base/box.h>

namespace MusselBed {

class FunctionalResponseGB : public base::Box
{
public:
    FunctionalResponseGB(QString name, QObject *parent);
    void update();

private:
    // inputs
    double apparency, resourceDensity, demand,
        egestionRatio, respiration,
        conversionCost;
    bool truncateSupply, costOnRespiration;


    // outputs
    double supply, sdRatio, totalDemand, totalSupply, egestion,
        attacksPerHost, numHostsAttacked, propHostsAttacked;

    // methods
    static double f(double demand, double resource);
    static double divBounded(double x, double y, double bound);
};

} //namespace


#endif
