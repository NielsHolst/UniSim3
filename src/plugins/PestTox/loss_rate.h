/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef LOSS_RATE_H
#define LOSS_RATE_H

#include <base/box.h>

namespace PestTox {

class LossRate : public base::Box
{
public:
    LossRate(QString name, Box *parent);
    void reset() final;
    void update() final;
private:
    // Output
    double instantaneous, finite;
    // Methods
    virtual double computeInstantaneous() = 0;
};

} //namespace


#endif
