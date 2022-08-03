/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef COMPARTMENT_H
#define COMPARTMENT_H

#include <base/box.h>

namespace PestTox {

class Compartment : public base::Box
{
public:
    Compartment(QString name, Box *parent);
    void update();

private:
    // Input
    double gain, loss;

    // Output
    double load;
};

} //namespace


#endif
