/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BETA_RATE_H
#define BETA_RATE_H
#include <base/box.h>

namespace bmsb {

class BetaRate : public base::Box
{
public:
   BetaRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dailytempC, mu, Tb, Tc, aa, bb ;

    // Data
    double ovprate;
};

}

#endif
