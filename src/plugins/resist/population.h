/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef POPULATION_H
#define POPULATION_H
#include <base/box.h>

namespace resist {

class Population : public base::Box
{
public:
    Population(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double Ninit, K, R, survival, Ntotal;
    int steps;
    // Outputs
    double N;
};

}

#endif
