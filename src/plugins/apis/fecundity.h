/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Merijn Moens, Aarhus University [merijnmoens at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FECUNDITY_H
#define FECUNDITY_H
#include <base/box.h>

namespace apis {

class Fecundity : public base::Box
{
public:
    Fecundity(QString name, QObject *parent);
    void update();
private:
    // input
    double slope, intercept, brood, workers, maxFecundity, timeStepDays;
    // output
    double value, rate;
};

}

#endif
