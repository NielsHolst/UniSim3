/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef GROWTH_H
#define GROWTH_H
#include <base/box.h>

namespace coffee {

class Growth : public base::Box
{
public: 
    Growth(QString name, QObject *parent);
    void update();
private:
    // Inputs
    double temperature, parAverage30Days, parAverage, parIntercepted, transpirationRatio, growthStage,
        airCo2, k, convEfficiency,
        sinkLeaf, sinkWoody, sinkRoot,
        sinkBerryMax, sinkBerryCoef;
    // Outputs
    double
        sinkBerry,
        gTotalC,
        gLeafC, gWoodyC, gRootC, gBerryC;



};

} //namespace
#endif
