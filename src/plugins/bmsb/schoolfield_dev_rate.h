/* Copyright 2018-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SCHOOLFIELD_DEV_RATE_H
#define SCHOOLFIELD_DEV_RATE_H
#include <base/box.h>

namespace bmsb {

class SchoolFieldDevRate : public base::Box
{
public:
    SchoolFieldDevRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double TPhi, dailytempC, rhoPhi, HA, HL,  HH, TL, TH, R  ;

    // Data
    double numerator, denominator, devrate;
};

}

#endif
