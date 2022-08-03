/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LIN_DEV_RATE_H
#define LIN_DEV_RATE_H
#include <base/box.h>

namespace bmsb {

class LinDevRate : public base::Box
{
public:
    LinDevRate(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dailytempC, a0, a1;

    // Data
    double lineardevrate;
};

}

#endif
