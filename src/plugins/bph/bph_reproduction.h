/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BPH_REPRODUCTION_H
#define BPH_REPRODUCTION_H
#include <base/box.h>

namespace bph {

class BphReproduction : public base::Box
{
public:
    BphReproduction(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double max, vegetative, reproductive, ripening, repScale, ripScale;
    // Data
    double value;
};

}

#endif
