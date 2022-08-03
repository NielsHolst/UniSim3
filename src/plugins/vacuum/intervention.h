/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Emily Bick, University of California [enbick at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef  INTERVENTION_H
#define  INTERVENTION_H
#include <base/box.h>

namespace vacuum {

class Intervention : public base::Box
{
public:
     Intervention(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    int dayValue;
    int interval;
    double efficacy;
    // Outputs
    double mortality;
};

}

#endif
