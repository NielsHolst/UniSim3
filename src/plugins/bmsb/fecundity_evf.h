/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FECUNDITY_EVF_H
#define FECUNDITY_EVF_H
#include <base/box.h>

namespace bmsb {

class FecundityEVF : public base::Box
{
public:
    FecundityEVF(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double dailytempC, a, b, k;

    // Data
    double fecundity;
};

}

#endif
