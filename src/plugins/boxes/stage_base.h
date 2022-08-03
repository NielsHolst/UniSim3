/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef STAGE_BASE_H
#define STAGE_BASE_H

#include <QVector>
#include <base/box.h>
#include "distributed_delay_base.h"

namespace boxes {

class StageBase : public base::Box
{
public:
    StageBase(QString name, Box *parent);
    virtual ~StageBase();
    void reset() final;
    virtual void myReset() = 0;

    QVector<double> &data();
    virtual void createDistributedDelay() = 0;

protected:
    // inputs
    int k;
    double initial, duration, timeStep, growthFactor, sdRatio, instantLossRate, zeroLimit;
    QVector<double> phaseInflow;

    // outputs
    double content, growth,
        inflowSum, inflowTotal,
        phaseInflowSum, phaseInflowTotal,
        outflowSum, outflowTotal,
        phaseOutflowSum, phaseOutflowTotal;
    QVector<double> phaseOutflow;

    // methods
    void resetOutputsToZero();
    void applyInstantMortality(double mortality);

    // data
    DistributedDelayBase *_ddBase;
};

}
#endif
