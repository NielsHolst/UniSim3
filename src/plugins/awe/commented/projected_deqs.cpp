/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "projected_deqs.h"

using namespace base;

namespace awe {

PUBLISH(ProjectedDeqs)

ProjectedDeqs::ProjectedDeqs(QString name, QObject *parent)
    : Box(name,parent)
{
    Input(seedlingDeqs).imports("seedling/densityEqs[content]");
    Input(juvenileDeqs).imports("juveile/densityEqs[content]");
    Input(matureDeqs).imports("mature/densityEqs[content]");
    Input(matureDeqsOutflowTotal).imports("mature/densityEqs[outflowTotal]");
    Output(total);
}

void ProjectedDeqs::reset() {
    _prevOutflowTotal = 0.;
}

void ProjectedDeqs::update() {
    total = seedlingDeqs + juvenileDeqs + matureDeqs + matureDeqsOutflowTotal - _prevOutflowTotal;
}

}
