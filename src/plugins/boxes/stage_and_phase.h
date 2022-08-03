/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef STAGE_AND_PHASE_H
#define STAGE_AND_PHASE_H

#include "distributed_delay_2d.h"
#include "stage_base.h"

namespace boxes {

class DistributedDelay2D;

class StageAndPhase : public StageBase
{
public:
    StageAndPhase(QString name, Box *parent);
    // standard methods
    void myReset();
    void update();

    // special methods
    void createDistributedDelay();

private:
    // inputs
    int phaseK;
    double phaseDuration, phaseTimeStep, scalarInflow;
    QVector<double> inflow;

    // outputs
    QVector<double> outflow;

    // data
    bool _firstUpdate;
    DistributedDelay2D *_dd;
};

}
#endif
