/* Copyright 2018-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef EXPPOLY4_MORT_RATE_H
#define EXPPOLY4_MORT_RATE_H
#include <base/box.h>

namespace bmsb {

class ExpPoly4MortRate : public base::Box
{
public:
    ExpPoly4MortRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dailytempC, a0, a1, a2, a3, a4  ;

    // Data
    double survrate, mortrate;
};

}

#endif
