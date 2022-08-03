/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/test_num.h>
#include <base/vector_op.h>
#include "gene_frequencies.h"

using namespace base;

namespace resist {

PUBLISH(GeneFrequencies)

GeneFrequencies::GeneFrequencies(QString name, QObject *parent)
    : Box(name, parent){
    Input(N).help("Population size vector (ss, sr, rr)");
    Input(step).imports("/.[step]");
    Input(maxStep).imports("Simulation::*[steps]");
    Input(rThreshold).help("Threshold at which to register r frequenyc");
    Output(ss).help("Frequency ss genotype [0;1]");
    Output(sr).help("Frequency ss genotype [0;1]");
    Output(rr).help("Frequency ss genotype [0;1]");
    Output(s).help("Frequency s allele [0;1]");
    Output(r).help("Frequency r allele [0;1]");
    Output(thresholdGen).help("Generation in which threshold was reached or maxStep");
    Output(thresholdPassed).help("Was threshold passed?");
}

void GeneFrequencies::reset() {
    thresholdGen = maxStep;
    thresholdPassed = false;
}

void GeneFrequencies::update() {
    // Compute genotype proportions
    double sum = vector_op::sum(N);
    if (sum == 0)
        ss = sr = rr = r = s = 0.;
    else {
        ss = N[0]/sum;
        sr = N[1]/sum;
        rr = N[2]/sum;
        s = ss + sr/2;
        r = rr + sr/2;
    }
    if (step>0 && r>rThreshold && !thresholdPassed) {
        thresholdGen = step;
        thresholdPassed = true;
    }
}

}
