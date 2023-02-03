/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RUNNING_AVERAGE_STEPS_H
#define RUNNING_AVERAGE_STEPS_H
#include "running_sum_steps.h"

namespace boxes {

class RunningAverageSteps : public RunningSumSteps
{
public: 
    RunningAverageSteps(QString name, Box *parent);
    void update();
};

} //namespace
#endif
