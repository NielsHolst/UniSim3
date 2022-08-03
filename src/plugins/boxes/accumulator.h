/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H

#include <base/box.h>

namespace boxes {

class Accumulator : public base::Box
{
public:
    Accumulator(QString name, Box *parent);
    void reset();
    void update();

private:
    // Inputs
    double initial, change, minValue, maxValue;
    // Outputs
    double value;
};
} //namespace


#endif
