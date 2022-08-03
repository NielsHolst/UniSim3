/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef EXTREMEVAL_FUNC_H
#define EXTREMEVAL_FUNC_H
#include <base/box.h>

namespace bmsb {

class ExtremeValFunc : public base::Box
{
public:
    ExtremeValFunc(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dailytempC, k1, Tmax, rho;

    // Data
    double evfout;
};

}

#endif
