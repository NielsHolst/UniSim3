/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HONEY_DEMAND_RESPIRATION_H
#define HONEY_DEMAND_RESPIRATION_H
#include <base/box.h>

namespace apis {

class HoneyDemandRespiration : public base::Box
{
public:
    HoneyDemandRespiration(QString name, QObject *parent);
    void reset();
    void update();
private:
    // input
    double workers, respirationRate, timeStepSecs;
    // output
    double value, rate, total;
};

}

#endif
