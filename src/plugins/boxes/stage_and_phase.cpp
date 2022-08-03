/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <iostream>
#include <cmath>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include <base/vector_op.h>
#include "distributed_delay_2d.h"
#include "stage_and_phase.h"

using namespace base;

namespace boxes {

PUBLISH(StageAndPhase)

StageAndPhase::StageAndPhase(QString name, Box *parent)
    : StageBase(name, parent), _firstUpdate(true)
{
    help("models a 2-D distributed delay");
    Input(inflow).help("Amount of inflow (vector)");
    Input(scalarInflow).help("Amount of inflow (scalar)");
    Input(phaseK).equals(30).help("Phase distribution parameter");
    Input(phaseDuration).equals(100.).help("Average delay between inflow and outflow through phase");
    Input(phaseTimeStep).equals(1).help("Time step for phase development");
    Output(outflow).help("Stage outflow cohorts (vector)");
}

void StageAndPhase::createDistributedDelay() {
    DistributedDelay2D::FixedParameters p;
    if (duration <= 0.)
        ThrowException("duration <= 0").value(duration).context(this);
    if (phaseDuration  <= 0.)
        ThrowException("phaseDuration <= 0").value(phaseDuration).context(this);
    p.L1 = duration;
    p.L2 = phaseDuration;
    p.k1 = k;
    p.k2 = phaseK;
    p.minIter = 1;
    _ddBase = _dd = new DistributedDelay2D(p, this);
}

void StageAndPhase::myReset() {
    content = initial;
    inflow.resize(phaseK);
    outflow.resize(phaseK);
    outflow.fill(0.);
    _firstUpdate = true;
    _dd->resize(k, phaseK);
    update();   // Otherwise, outflow will be one simulation step late
}

void StageAndPhase::update() {
    if (inflow.size() != phaseK) {
        QString msg("inflow's size (phaseK=%1) does not match the size of the recipient (phaseK=%2)");
        ThrowException(msg.arg(inflow.size()).arg(phaseK)).context( this);
    }
    if (phaseInflow.size() != k) {
        QString msg("phaseInflow's size (k=%1) does not match the size of the recipient (k=%2)");
        ThrowException(msg.arg(phaseInflow.size()).arg(k)).context( this);
    }

    applyInstantMortality(instantLossRate);

    inflowSum = vector_op::sum(inflow);
    inflowTotal += inflowSum;
    phaseInflowSum = vector_op::sum(phaseInflow);
    phaseInflowTotal += phaseInflowSum;
    if (_firstUpdate)
        inflow[0] += initial;
    inflow[0] += scalarInflow;


    // Replace zero time step with neglible time step
    if (timeStep == 0.)
        timeStep = 1e-12;
    if (phaseTimeStep == 0.)
        phaseTimeStep = 1e-12;

    // Replace zero growth with neglible growth
    if (growthFactor == 0.)
        growthFactor = 1e-12;

    DistributedDelay2D::UpdateParameters p;
    p.dt1 = timeStep;
    p.dt2 = phaseTimeStep;
    p.inflow1 = &inflow;
    p.inflow2 = &phaseInflow;
    p.fgr1 = growthFactor;
    p.fgr2 = 1.;
    _dd->update(p);

    content = _dd->content();
    outflow = _dd->state().outflow1;
    outflowSum = vector_op::sum(outflow);
    outflowTotal += outflowSum;
    phaseOutflow = _dd->state().outflow2;
    phaseOutflowSum = vector_op::sum(phaseOutflow);
    phaseOutflowTotal += phaseOutflowSum;
    growth = _dd->state().growthRate;
    if (_firstUpdate) {
        inflow[0] -= initial;
        _firstUpdate = false;
    }
    if (content>0. && content<zeroLimit) {
        applyInstantMortality(1.);
        content = _dd->content();
    }
}

} // namespace
