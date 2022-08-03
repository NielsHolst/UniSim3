/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/computation.h>
#include <base/publish.h>
#include "computation.h"

using namespace base;

namespace boxes {

PUBLISH(Computation)

Computation::Computation(QString name, Box *parent)    : Box(name, parent)
{
    help("provides information on the computation schedule");
    Output(step).help("Name of current computation step");
    updateStep();
}

void Computation::amend() {
    update();
}

void Computation::initialize() {
    update();
}

void Computation::reset() {
    update();
}

void Computation::update() {
    updateStep();
}

void Computation::cleanup() {
    update();
}

void Computation::debrief() {
    update();
}

void Computation::updateStep() {
    step = base::Computation::toString(base::Computation::currentStep());
}

} //namespace

