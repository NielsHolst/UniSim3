/* Copyright 2018-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SCHOOLFIELDLOW_DEV_RATE_H
#define SCHOOLFIELDLOW_DEV_RATE_H
#include <base/box.h>

namespace bmsb {

class SchoolFieldLowDevRate : public base::Box
{
public:
    SchoolFieldLowDevRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double TPhi, dailytempC, rhoPhi, HA,  HL, TL, R  ;

    // Data
    double numerator, denominator, devrate;
};

}

#endif
