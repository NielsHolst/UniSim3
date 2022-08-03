/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "spray_survival.h"

using namespace base;

namespace resist {

PUBLISH(SpraySurvival)

SpraySurvival::SpraySurvival(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(coverage).help("Spray coverage [0;1]");
    Input(maxMortality).help("Max. mortality on SS genotype caused by spraying [0;1]");
    Input(slope).equals(4).help("Initial slope of mortality on coverage");
    Input(resistanceGain).help("Resistance gained by R allele");
    Output(ss).help("Survival of SS genotype");
    Output(sr).help("Survival of SR genotype");
    Output(rr).help("Survival of RR genotype");
    Output(values).help("Survival vector (ss, sr, rr)");
    values.resize(3);
}

void SpraySurvival::reset() {
    if (slope <= 0)
        ThrowException("Slope must be positive").value(slope).context(this);
    ss = sr = rr = 1.;
    values.fill(1.);
}

void SpraySurvival::update() {
    double mortality = maxMortality*(1. - exp(-slope*coverage/maxMortality)),
           g = 1. - resistanceGain;
    values[0] = ss = 1. - mortality;
    values[1] = sr = 1. - mortality*g;
    values[2] = rr = 1. - mortality*g*g;
}

}
