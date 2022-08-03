/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <iostream>
#include <cmath>
#include <QString>
#include <QTextStream>
#include <base/exception.h>
#include <base/test_num.h>
#include "stage_base.h"

using namespace base;

namespace boxes {

StageBase::StageBase(QString name, Box *parent)
    : Box(name, parent), _ddBase(nullptr)
{
    Input(initial).help("Initial amount of inflow");
    Input(k).equals(30).help("Distribution parameter; use small k for larger dispersion in output");
    Input(duration).equals(100).help("Average delay between inflow and outflow");
    Input(timeStep).equals(1).help("Time step");
    Input(growthFactor).equals(1).help("Factor by which outflow will be scaled relative to inflow");
    Input(instantLossRate).help("Mortality applied before inflow is added [0..1]");
    Input(phaseInflow).help("Phase inflow cohorts (vector)");
    Input(zeroLimit).equals(1e-12).help("Cut contents to zero when total is less than this");

    Output(content).help("Total content inside the stage");
    NamedOutput("value", content).help("Synonyme for 'content'");
    Output(inflowSum).help("Sum of current stage inflow (scalar)");
    Output(inflowTotal).help("Accumulated total stage inflow (scalar)");
    Output(phaseInflowSum).help("Sum of current phase inflow cohorts (scalar)");
    Output(phaseInflowTotal).help("Accumulated total phase inflow (scalar)");
    Output(outflowSum).help("Sum of current stage outflow (scalar)");
    Output(outflowTotal).help("Accumulated total stage outflow (scalar)");
    Output(phaseOutflowSum).help("Sum of current phase outflow cohorts (scalar)");
    Output(phaseOutflowTotal).help("Accumulated total phase outflow (scalar)");
    Output(phaseOutflow).help("Phase outflow cohorts (vector)");
    Output(growth).help("Change in content+outflow during this time step");
}

StageBase::~StageBase() {
    delete _ddBase;
}

void StageBase::reset()
{
    if (k <= 0)
        ThrowException("k must be > 0").value(k).context(this);
    if (duration <= 0)
        ThrowException("Duration must be > 0").value(duration).context(this);
    delete _ddBase;
    createDistributedDelay();
    phaseInflow.resize(k);
    resetOutputsToZero();
    myReset();
}

void StageBase::resetOutputsToZero() {
    content = growth =
    inflowSum = inflowTotal =
    phaseInflowSum = phaseInflowTotal =
    outflowSum = outflowTotal =
    phaseOutflowSum = phaseOutflowTotal = 0;
    phaseOutflow.resize(k);
    phaseOutflow.fill(0.);
}

void StageBase::applyInstantMortality(double mortality) {
    TestNum::snapToZero(mortality);
    TestNum::snapTo(mortality, 1.);
    double survival = 1. - mortality;
    if (survival < 0. || survival > 1.)
       ThrowException("Mortality must be in the range [0;1]").value(mortality).context(this);
    _ddBase->scale(survival);
}


QVector<double> &StageBase::data() {
    return _ddBase->data();
}

} // namespace
