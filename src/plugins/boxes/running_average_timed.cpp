/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "running_average_timed.h"

using namespace base;
using boost::numeric_cast;

namespace boxes {

PUBLISH(RunningAverageTimed)

RunningAverageTimed::RunningAverageTimed(QString name, Box *parent)
    : RunningSumTimed(name, parent)
{
    help("computes running average in a time window");
}

void RunningAverageTimed::update() {
    RunningSumTimed::update();
    value /= bufferSize();
}

} //namespace

