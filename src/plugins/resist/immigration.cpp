/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "immigration.h"

using namespace base;

namespace resist {

PUBLISH(Immigration)

Immigration::Immigration(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(q).help("Frequency of resistance allele [0;1]");
    Input(proportion).help("Proportion of carrying capacity [0;1]");
    Input(K).help("Carrying capacity");
    Output(ss).help("Number of SS immigrants");
    Output(sr).help("Number of SR immigrants");
    Output(rr).help("Number of RR immigrants");
    Output(values).help("Immigration vector (ss, sr, rr)");
    values.resize(3);
}

void Immigration::reset() {
    if (q<0 || q>1)
        Exception("'q' must be inside [0;1]").value(q).context(this);
    if (proportion<0 || proportion>1)
        Exception("'proportion' must be inside [0;1]").value(proportion).context(this);
    update();
}

void Immigration::update() {
    double p = 1. - q;
    values[0] = ss = p*p*K*proportion;
    values[1] = sr = p*q*K*proportion;
    values[2] = rr = q*q*K*proportion;
}

}
