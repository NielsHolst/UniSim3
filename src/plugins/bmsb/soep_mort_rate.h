/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SOEP_MORT_RATE_H
#define SOEP_MORT_RATE_H
#include <base/box.h>

namespace bmsb {

class SOEPMortRate : public base::Box
{
public:
    SOEPMortRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double a0, a1, a2, dailytempC;

    // Data
    double mortrate;
};

}

#endif
