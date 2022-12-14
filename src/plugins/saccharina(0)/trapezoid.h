/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef TRAPEZOID_H
#define TRAPEZOID_H
#include <base/box.h>

namespace saccharina {

class Trapezoid : public base::Box
{
public:
    Trapezoid(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double T, T0, Topt1, Topt2, Tmax;
    // Outputs
    double value;
};

}

#endif
