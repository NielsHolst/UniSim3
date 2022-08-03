/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HONEY_SUPPLY_GROWTH_H
#define HONEY_SUPPLY_GROWTH_H
#include <base/box.h>

namespace apis {

class HoneySupplyGrowth : public base::Box
{
public:
    HoneySupplyGrowth(QString name, QObject *parent);
    void update();
private:
    // input
    QVector<double> cohortDemands;
    double cost;
    // output
    QVector<double> cohortSupplies ;
};

}

#endif
