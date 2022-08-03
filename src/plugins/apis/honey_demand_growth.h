/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HONEY_DEMAND_GROWTH_H
#define HONEY_DEMAND_GROWTH_H
#include <base/box.h>

namespace apis {

class HoneyDemandGrowth : public base::Box
{
public:
    HoneyDemandGrowth(QString name, QObject *parent);
    void reset();
    void update();
private:
    // input
    QVector<double> cohortNumbers, cohortDemands;
    double cost, timeStepSecs;
    // output
    double value, rate, netTotal, costTotal;
};

}

#endif
