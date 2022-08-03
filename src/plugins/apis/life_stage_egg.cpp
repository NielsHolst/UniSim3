/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <algorithm>
#include <base/publish.h>
#include <base/vector_op.h>
#include "life_stage_egg.h"

using namespace base;

namespace apis {

PUBLISH(LifeStageEgg)

LifeStageEgg::LifeStageEgg(QString name, QObject *parent)
    : Box(name, parent),
    _number(&number),
    _age(&age)
{
    Input(timeStepDays).imports("calendar[timeStepDays]");
    Input(Pmorph).imports("./Pmorph[dy]");
    Input(bufferSize).equals(30);
    Input(numberInit).equals(0);
    Input(numberNew);
    Output(number);
    Output(age);
    Output(numberSum);
    Output(numberMorphed);
    Output(numberLeaked);
}

void LifeStageEgg::initialize() {
    _number.resize(bufferSize);
    _age.resize(bufferSize);
}

void LifeStageEgg::reset() {
    _number.clear();
    _age.clear();

    _number.push(numberInit);
    _age.push(0);
    updateStatistics();
}

void LifeStageEgg::update() {
    // Metamorphosis
    Cohorts numberMorphedCohorts(bufferSize);
    vector_op::multiply(numberMorphedCohorts, number, Pmorph),
    vector_op::minus(number, numberMorphedCohorts);
    numberMorphed = vector_op::sum(numberMorphedCohorts);
    // Cohorts development
    vector_op::plus(age, timeStepDays);
    numberLeaked = _number.push(numberNew);
    _age.push(0);
    updateStatistics();
}

void LifeStageEgg::updateStatistics() {
    _number.updateStatistics();
    numberSum = _number.sum();
}

}
