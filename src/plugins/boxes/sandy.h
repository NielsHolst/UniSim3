/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SANDY_H
#define SANDY_H
#include <base/box.h>

namespace boxes {

class Sandy : public base::Box
{
public:
    Sandy(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double xMin, xMax, x, yMax, a, b;
    // Outputs
    double value;
    // Data
    double C;
};

}

#endif
