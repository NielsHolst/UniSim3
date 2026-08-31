/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef DAY_DEGREES
#define DAY_DEGREES

#include <base/physiological_time.h>

namespace boxes {

class UNI_API DayDegrees : public base::PhysiologicalTime
{
public: 
    DayDegrees(QString name, Box *parent);
    void reset() override;
    void updateStep() override;
private:
    // Inputs
    double T0, Topt, Tmax;
};

} //namespace
#endif
