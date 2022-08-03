/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PHYSIOLOGICAL_TIME
#define BASE_PHYSIOLOGICAL_TIME
#include "box.h"

namespace base {

class PhysiologicalTime : public Box
{
public:
    PhysiologicalTime(QString name, Box *parent);
    void update() final;
    virtual void updateStep() = 0;
protected:
    // Inputs
    double T, timeStepDays;
    bool resetTotal, isTicking;
    // Outputs
    double step, total;
};

}
#endif
