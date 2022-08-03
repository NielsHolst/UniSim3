/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Andrew Antaya, University of Arizona [aantaya at email.arizona.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef Fecundity_H
#define Fecundity_H
#include <base/box.h>

namespace pinacate {

class Fecundity : public base::Box
{
public:
    Fecundity(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double density;
    double percapita;
    double availability;
    // Outputs
    double value;
};

}

#endif
