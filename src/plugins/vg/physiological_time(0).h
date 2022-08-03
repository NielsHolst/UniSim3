/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PHYSIOLOGICAL_TIME
#define PHYSIOLOGICAL_TIME

#include <base/box.h>

namespace vg {

class PhysiologicalTime : public base::Box
{
public: 
    PhysiologicalTime(QString name, Box *parent);
    void update();
    virtual double calcDailyTimeStep() = 0;

private:
    // Inputs
    double step, total, timeStepDays;
    bool doReset, isTicking;
};

} //namespace
#endif
