/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef DAY_DEGREES
#define DAY_DEGREES
#include "physiological_time.h"

namespace vg {

class DayDegrees : public PhysiologicalTime
{
public: 
    DayDegrees(QString name, Box *parent);
    double calcDailyTimeStep();
protected:
    // Inputs
    double T0, Topt, Tmax, T;
};

} //namespace
#endif
