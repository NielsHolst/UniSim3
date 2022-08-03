/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef IMMIGRANT_OVIPOSITION_H
#define IMMIGRANT_OVIPOSITION_H
#include <base/box.h>

namespace bph {

class ImmigrantOviposition : public base::Box
{
public:
    ImmigrantOviposition(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double vegetative, immigrants, fecundity;
    // Data
    double value;
};

}

#endif
