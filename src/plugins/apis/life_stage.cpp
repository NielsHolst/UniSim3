/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <algorithm>
#include <boost/numeric/conversion/cast.hpp>
#include <base/publish.h>
#include <base/test_num.h>
#include <base/vector_op.h>
#include "life_stage.h"

using boost::numeric_cast;
using namespace base;
using namespace::vector_op;

namespace apis {

PUBLISH(LifeStage)

LifeStage::LifeStage(QString name, QObject *parent)
    : Box(name, parent),
    _number(&number),
    _mass(&mass),
    _age(&age)
{
    Input(maxAge).equals(30).help("Size of cohort buffer (days)");
    Input(timeStepDays).imports("calendar[timeStepDays]");
    Input(Pmorph).imports("./Pmorph[dy]");
    Input(massIncrement).help("Vector or scalar with increment(s) added to individual mass of each cohort in this time step (mg)");
    Input(numberInit).help("Initial number of individuals in the first cohort");
    Input(massInit).help("Initial average mass of individuals in the first cohort");;
    Input(numberNew).help("Number of individuals in the new cohort created in this time step");
    Input(massNew).help("Individual mass of the new cohort created in this time step (mg)");
    Output(number).help("Number of individuals in each cohort");
    Output(mass).help("Individual mass of each cohort (mg)");
    Output(age).help("Age of each cohort (days)");
    Output(numberSum).help("Total number of individuals");
    Output(massAverage).help("Weighted average of individual mass (mg)");
    Output(ageAverage).help("Weighted average of cohort ages (days)");;
    Output(numberMorphed).help("Number of indiviuals morphing into the next life stage");
    Output(massMorphed).help("Weighted individual mass of indiviuals morphing into the next life stage");
    Output(numberLeaked).help("Number of individuals lost because of buffer overrun; increase maxAge to avoid this");
    Output(massLeaked).help("Mass of individuals lost because of buffer overrun (mg); increase maxAge to avoid this");
}

void LifeStage::initialize() {
    if (TestNum::gt(timeStepDays,1.))
        ThrowException("Time step longer than one day not allowed").value(timeStepDays);
    _bufferSize = numeric_cast<int>(maxAge/timeStepDays);

    _number.resize(_bufferSize);
    _mass.resize(_bufferSize);
    _age.resize(_bufferSize);
}

void LifeStage::reset() {
    _number.clear();
    _mass.clear();
    _age.clear();

    _number.push(numberInit);
    _mass.push(massInit);
    _age.push(0);
    updateStatistics();
}

void LifeStage::update() {
    // Metamorphosis
    Cohorts numberMorphedCohorts;

    product(numberMorphedCohorts,
            number, Pmorph, this);
    subtract(number,
             numberMorphedCohorts, this);
    numberMorphed = sum(numberMorphedCohorts);

    // Growth
    if (!massIncrement.isEmpty())
        add(mass,
            massIncrement, this);
    massMorphed = weightedAverage(mass, numberMorphedCohorts, this);

    // Cohorts development
    add(age,
        timeStepDays);
    numberLeaked = _number.push(numberNew);
    massLeaked =_mass.push(massNew)*numberLeaked;
    _age.push(0);
    updateStatistics();
}

void LifeStage::updateStatistics() {
    numberSum = sum(number);
    massAverage = weightedAverage(mass, number, this);
    ageAverage = weightedAverage(age, number, this);
}

}
