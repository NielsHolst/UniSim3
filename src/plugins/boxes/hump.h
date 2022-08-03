/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HUMP_H
#define HUMP_H
#include <base/box.h>

namespace boxes {

class Hump : public base::Box
{
public: 
    Hump(QString name, Box *parent);
    // standard methods
    void initialize();
    void reset();
    void update();
private:
    // Input
    double x, x0, x1, ymin, ymax;
    bool ditch;
    // Output
    double value;
};

} //namespace
#endif
