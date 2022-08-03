/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DISTRIBUTED_DELAY_H
#define DISTRIBUTED_DELAY_H
#include "distributed_delay_base.h"

namespace base {
    class Box;
};

namespace boxes {

class DistributedDelay : public DistributedDelayBase
{
public:
    struct Parameters {
        double L;
        int k, minIter;
    };
    struct State {
        double outflowRate, growthRate;
        State() : outflowRate(0), growthRate(0) {}
    };

    DistributedDelay(const Parameters &p, base::Box *parent);
    DistributedDelay(const DistributedDelay &dd);
    void update(double inflow, double dt, double fgr);
    State state() const;
    void resize(int k);

private:
    // data
    Parameters p;
    State s;
};

}
#endif
