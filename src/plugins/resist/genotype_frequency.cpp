/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "genotype_frequency.h"

using namespace base;

namespace resist {

PUBLISH(GenotypeFrequency)

GenotypeFrequency::GenotypeFrequency(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(N).help("Population size of this genotype");
    Input(Ntotal).help("Total population size");;
    Output(value).help("Frequency of this genotype [0;1]");
}

void GenotypeFrequency::update() {
    value = (Ntotal != 0) ? N/Ntotal : 0;
}

}
