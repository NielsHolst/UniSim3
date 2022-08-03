/* Copyright 2020-2021 by
** Teis Boderskov,.Aarhus University [tebo@bios.au.dk] and
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] 
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef MATURITY_H
#define MATURITY_H
#include <base/box.h>

namespace saccharina {

class Maturity : public base::Box
{
public:
    Maturity(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double A, a50, aSlope, forced;
    // Outputs
    double value;
    // Data
    bool isForced;
};

}

#endif
