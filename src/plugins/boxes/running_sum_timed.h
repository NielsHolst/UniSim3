/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RUNNING_SUM_TIMED_H
#define RUNNING_SUM_TIMED_H
#include "running_base.h"

namespace boxes {

class RunningSumTimed : public RunningBase
{
public: 
    RunningSumTimed(QString name, Box *parent);
    void reset();
private:
    // Input
    double timeWindow, timeStepSecs;
    QString timeUnit;
    // Output
    int count;
protected:
    // Methods
    int bufferSize() const;
};

} //namespace
#endif
