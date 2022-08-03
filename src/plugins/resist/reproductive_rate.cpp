/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "reproductive_rate.h"

using namespace base;

namespace resist {

PUBLISH(ReproductiveRate)

ReproductiveRate::ReproductiveRate(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(R).help("Reproductive rate of SS genotype");
    Input(fitnessPenalty).help("Fitness cost of R allele [0;1]");
    Input(Ntotal).help("Total population size");
    Input(K).help("Carrying capacity");
    Output(ss).help("Net reproductive rate for SS genotype");
    Output(sr).help("Net reproductive rate for SR genotype");
    Output(rr).help("Net reproductive rate for RR genotype");
    Output(values).help("Net reproductive rate   vector (ss, sr, rr)");
    values.resize(3);
}

void ReproductiveRate::update() {
    values[0] = ss = (R!=0 && K!=0) ? exp(log(R)*(K-Ntotal)/K) : 0.;
    values[1] = sr = ss*(1.-fitnessPenalty);
    values[2] = rr = ss*(1.-fitnessPenalty)*(1.-fitnessPenalty);
}

}
