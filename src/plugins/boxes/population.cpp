/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <algorithm>
#include <base/exception.h>
#include <base/path.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "population.h"

using namespace base;
using namespace vector_op;

namespace boxes {

PUBLISH(Population)

Population::Population(QString name, Box *parent)
    : Box(name, parent),
      _cohorts(&cohorts),
      _age(&age)
{
    help("models population dynamics by cohorts (experimental)");
    Input(initial).equals(100);
    Input(ageIncrement);
    Input(bufferSize).equals(100);
    Input(firstCohortGain);
    Input(cohortsGain);
    Input(cohortsLoss);

    Output(lastCohortSpill);
    Output(cohorts);
    Output(age);
}

void Population::reset() {
    _cohorts.resize(bufferSize);
    _age.resize(bufferSize);

    _cohorts.push(initial);
    _age.push(0);

    if (!cohortsGain.isEmpty() && cohortsGain.size() != bufferSize) {
        QString msg{"cohortsGain must have same vector size (%1) as bufferSize (%2)"};
        ThrowException(msg.arg(cohortsGain.size()).arg(bufferSize));
    }
    if (!cohortsLoss.isEmpty() && cohortsLoss.size() != bufferSize) {
        QString msg{"cohortsLoss must have same vector size (%1) as bufferSize (%2)"};
        ThrowException(msg.arg(cohortsLoss.size()).arg(bufferSize));
    }
}

void Population::update() {
    if (!cohortsGain.isEmpty())
        add(cohorts,
            cohortsGain, this);
    if (!cohortsLoss.isEmpty())
        subtract(cohorts,
                 cohortsLoss, this);
    add(age,
        ageIncrement);
    lastCohortSpill = _cohorts.at(1);
    _cohorts.push(firstCohortGain);
    _age.push(0);
}



}
