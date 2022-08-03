/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BPH_SURVIVAL_H
#define BPH_SURVIVAL_H
#include <base/box.h>

namespace bph {

class BphSurvival : public base::Box
{
public:
    BphSurvival(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double density, atDensity, survival;
    // Outputs
    double value;
    // Data
    double _slope;
};

}

#endif
