/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Kayla Sale, University of Arizona [kayla.sale at email.arizona.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
// Description: pollinator class
#ifndef POLLINATOR_H
#define POLLINATOR_H
#include <base/box.h>

namespace pollnet {

class Pollinator : public base::Box
{
public:
    Pollinator(QString name, QObject *parent);
    void reset();
    void update(); // compute_new_B

private:

    // Inputs
    double initial_B;
    double CR; // consumption rate of pollinator on floral rewards
    double m; // body mass of pollinator
    bool is_vertebrate; // true if vertebrate

    // Outputs
    double B; // pollinator biomass
    double x; // metabolic rate of pollinator
    double y; // maximum consumption rate of pollinator eating floral rewards

    // Methods
    double compute_dBdt();

};

}

#endif
