/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef EXPPOLY4_SURV_RATE_H
#define EXPPOLY4_SURV_RATE_H
#include <base/box.h>

namespace bmsb {

class ExpPoly4SurvRate : public base::Box
{
public:
    ExpPoly4SurvRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dailytempC, a0, a1, a2, a3, a4  ;

    // Data
    double survrate;
};

}

#endif
