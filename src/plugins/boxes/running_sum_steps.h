/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef RUNNING_SUM_STEPS_H
#define RUNNING_SUM_STEPS_H
#include "running_base.h"

namespace boxes {

class RunningSumSteps : public RunningBase
{
public: 
    RunningSumSteps(QString name, Box *parent);
private:
    // Input
    int count;
protected:
    // Methods
    int bufferSize() const;
};

} //namespace
#endif
