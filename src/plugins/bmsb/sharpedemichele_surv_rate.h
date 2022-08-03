/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SHARPEDEMICHELE_SURV_RATE_H
#define SHARPEDEMICHELE_SURV_RATE_H
#include <base/box.h>

namespace bmsb {

class SharpeDeMicheleSurvRate : public base::Box
{
public:
    SharpeDeMicheleSurvRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double  dailytempC, Phi, HA, SL, SH, HL,  HH,  R  ;

    // Data
    double numerator, denominator, survrate;
};

}

#endif
