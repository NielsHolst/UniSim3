/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef RUNNING_AVERAGE_TIMED_H
#define RUNNING_AVERAGE_TIMED_H
#include "running_sum_timed.h"

namespace boxes {

class RunningAverageTimed : public RunningSumTimed
{
public: 
    RunningAverageTimed(QString name, Box *parent);
    void update();
};

} //namespace
#endif
