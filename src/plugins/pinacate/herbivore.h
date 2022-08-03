/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Andrew Antaya, University of Arizona [aantaya at email.arizona.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef Herbivore_H
#define Herbivore_H
#include <base/box.h>

namespace pinacate {

class Herbivore : public base::Box
{
public:
    Herbivore(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double weight;
    double popsize;
    // Outputs
    double value;
};

}

#endif
