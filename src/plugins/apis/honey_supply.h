/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HONEY_SUPPLY_H
#define HONEY_SUPPLY_H
#include <base/box.h>

namespace apis {

class HoneySupply : public base::Box
{
public:
    HoneySupply(QString name, QObject *parent);
    void update();
private:
    // input
    QVector<double> cohortNumbers;
    double cropVolume, handlingTime, conversionFactor,
        nectarFlow, timeStepSecs;
    // output
    double value, rate;
};

}

#endif
