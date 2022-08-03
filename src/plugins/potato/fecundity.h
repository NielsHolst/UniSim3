/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Isaac Kwesi Abuley , Aarhus University [ikabuley at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef FECUNDITY_H
#define FECUNDITY_H
#include <base/box.h>

namespace potato {

class Fecundity: public base::Box
{
public:
    Fecundity(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double R0, senescence;
    // Outputs
    double value;
};

}
#endif
