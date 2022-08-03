/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "check_vector.h"
#include "density.h"

using namespace base;

namespace resist {

PUBLISH(Density)

Density::Density(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(immigration).help("Immigrants vector (ss,sr,rr)");
    Input(reproduction).help("Reproduction vector (ss,sr,rr)");
    Input(survival).help("Survival vector by genotype (ss,sr,rr)");
    Input(step).imports("/.[step]");
    Input(maxStep).imports("Simulation::*[steps]");
    Input(densityThreshold).help("Threshold at which to register density");
    Output(ss).help("Density of ss genotype");
    Output(sr).help("Density of sr genotype");
    Output(rr).help("Density of rr genotype");
    Output(values).help("Density vector (ss, sr, rr)");
    Output(total).help("Total density");
    Output(thresholdGen).help("Generation in which threshold was reached or maxStep");
    Output(thresholdPassed).help("Was threshold passed?");
    values.resize(3);
}

void Density::reset() {
    ss = immigration[0];
    sr = immigration[1];
    rr = immigration[2];
    total = ss + sr + rr;
    thresholdGen = maxStep;
    thresholdPassed = false;
}

void Density::update() {
    CHECK_VECTOR_3(immigration);
    CHECK_VECTOR_3(reproduction);
    CHECK_VECTOR_3(survival);
    values[0] = ss = immigration[0] + survival[0]*reproduction[0];
    values[1] = sr = immigration[1] + survival[1]*reproduction[1];
    values[2] = rr = immigration[2] + survival[2]*reproduction[2];
    total = ss + sr + rr;
    if (total>densityThreshold && thresholdGen==0 ) {
        thresholdGen = step;
    }
    if (step>0 && total>densityThreshold && !thresholdPassed) {
        thresholdGen = step;
        thresholdPassed = true;
    }
}

}
